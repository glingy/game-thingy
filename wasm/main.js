var span = document.getElementById('view')

;(async () => {
  let ws = new WebSocket("wss://echo.websocket.org")
  ws.binaryType = "arraybuffer"
  let mem = 0


  let asm = window.asm = (await WebAssembly.instantiate(
    (await (await fetch("worker.wasm")).arrayBuffer()), 
    {
      env: {
        printMath: (a, b, sum) => span.innerText = `${a} + ${b} = ${sum}`,
        send: () => ws.send(asm.memory.buffer.slice(asm.outMsg, asm.outMsg + asm.msgLen())),
        print: (msg) => console.log(msg)
      }
    }
  )).instance.exports

  mem = new Uint8Array(asm.memory.buffer)

  ws.onmessage = (msg) => {
    mem.set(new Uint8Array(msg.data.slice(0, asm.msgLen())), asm.inMsg)
    asm.recvMessage()
  }
})()



let i = {
  hello: 0,
  test: 1,
  0: 3
}

// 0 - String, # - number of bits to read into parameter...?
// Decorators? oooh...
// YES! Decorators! bingo!

for (const key in object) {
  if (Object.hasOwnProperty.call(object, key)) {
    const element = object[key];
    
  }
}

let msg = new Uint16Array(8);
let j = {}

let bits = 0;
let short = 0;

for ((key, bits) of i) {
  if (i.hasOwnProperty(key)) {
    switch (bits) {
      case STRING:
        let len = msg.indexOf(0);
        j[key] = String.fromCharCode.apply(null, msg.slice(0, len));
        msg = msg.slice(len);
        break;
      case BOOLEAN:
      case INT16:
        j[key] = msg[0];
        msg = msg.slice(1);
        break;
      case INT32:
        j[key] = (msg[0] << 16) | msg[1];
        msg = msg.slice(2);
        break;
    }
  }
}