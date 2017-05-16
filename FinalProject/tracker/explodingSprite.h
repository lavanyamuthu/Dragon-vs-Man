#include <vector>
#include <list>
#include "chunk.h"

class ExplodingSprite : public Sprite {
public:
  ExplodingSprite(const Sprite& s);
  ExplodingSprite(const std::string& n,const Vector2f& pos,const Vector2f& vel,const Frame*);
  ~ExplodingSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void makeChunks(unsigned int);
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
  const std::string& getNameofExplodingSprite() const { return name; }
private:
  std::string name;
  std::list<Chunk> chunks; // An ExplodingSprite is a list of sprite chunks
  std::list<Chunk> freeList; // When a chunk gets out of range it goes here
  std::vector<Frame*> frames; // Each chunk has a Frame
  ExplodingSprite(const ExplodingSprite&); // Explicit disallow (Item 6)
  ExplodingSprite& operator=(const ExplodingSprite&); // (Item 6)
};
