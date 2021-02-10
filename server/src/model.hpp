#ifndef MODEL_H
#define MODEL_H

enum Type {
  ULONG = 0,
  STRING,
  INT,
  NUM_TYPES
};

const char * TYPES[NUM_TYPES] = {
  "UNSIGNED LONG",
  "VARCHAR(255)",
  "INT"
};

struct Column {
  Type type;
  const char * name;
};

typedef Column Schema[];

template <class T> class Model {
public:
  virtual void init() = 0;
  virtual void table() = 0;

  T fromId(unsigned long id);

protected:
  virtual T createFromRow() = 0;
  static void init(Schema schema);

private:

};

#endif