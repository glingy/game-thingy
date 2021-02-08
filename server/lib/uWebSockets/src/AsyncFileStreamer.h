#include <filesystem>

struct AsyncFileStreamer {

    std::map<std::string_view, AsyncFileReader *> asyncFileReaders;
    std::string root;

    AsyncFileStreamer(std::string root) : root(root) {
        // for all files in this path, init the map of AsyncFileReaders
        updateRootCache();
    }

    bool hasUrl(std::string_view url) {
        return asyncFileReaders.find(url) == asyncFileReaders.end();
    }

    void updateRootCache() {
        // todo: if the root folder changes, we want to reload the cache
        for(auto &p : std::filesystem::recursive_directory_iterator(root)) {
            std::string url = p.path().string().substr(root.length());
            if (url == "/index.html") {
                url = "/";
            }

            char *key = new char[url.length()];
            memcpy(key, url.data(), url.length());
            asyncFileReaders[std::string_view(key, url.length())] = new AsyncFileReader(p.path().string());
        }
    }

    template <bool SSL>
    bool streamFile(uWS::HttpResponse<SSL> *res, std::string_view url) {
        auto it = asyncFileReaders.find(url);
        if (it == asyncFileReaders.end()) {
            std::cout << "Did not find file: " << url << std::endl;
            return false;
        } else {
            const char * contentType;
            
            if (url.ends_with(".html")) {
              contentType = "text/html";
            } else if (url.ends_with(".css")) {
              contentType = "text/css";
            } else if (url.ends_with(".js")) {
              contentType = "text/javascript";
            } else {
              contentType = "text/txt";
            }
            
            res->writeHeader("Content-Type", contentType);

            streamFile(res, it->second);
            return true;
        }
    }

    template <bool SSL>
    static void streamFile(uWS::HttpResponse<SSL> *res, AsyncFileReader *asyncFileReader) {
        /* Peek from cache */
        std::string_view chunk = asyncFileReader->peek(res->getWriteOffset());
        if (!chunk.length() || res->tryEnd(chunk, asyncFileReader->getFileSize()).first) {
            /* Request new chunk */
            // todo: we need to abort this callback if peer closed!
            // this also means Loop::defer needs to support aborting (functions should embedd an atomic boolean abort or something)

            // Loop::defer(f) -> integer
            // Loop::abort(integer)

            // hmm? no?

            // us_socket_up_ref becaue we share ownership

            if (chunk.length() < asyncFileReader->getFileSize()) {
                asyncFileReader->request(res->getWriteOffset(), [res, asyncFileReader](std::string_view chunk) {
                    // check if we were closed in the mean time
                    //if (us_socket_is_closed()) {
                        // free it here
                        //return;
                    //}

                    /* We were aborted for some reason */
                    if (!chunk.length()) {
                        // todo: make sure to check for is_closed internally after all callbacks!
                        res->close();
                    } else {
                        AsyncFileStreamer::streamFile(res, asyncFileReader);
                    }
                });
            }
        } else {
            /* We failed writing everything, so let's continue when we can */
            res->onWritable([res, asyncFileReader](unsigned long) {

                AsyncFileStreamer::streamFile(res, asyncFileReader);
                // todo: I don't really know what this is supposed to mean?
                return false;
            })->onAborted([]() {
                std::cout << "ABORTED!" << std::endl;
            });
        }
    }
};
