#include "metadata.h"

using namespace std;

Metadata::Metadata(const Table &t)
    : t_(t)
{
    buffer = new char[PAGE_SIZE]; // may throw bad_alloc
    
    fs.open(t.name + ".dat", fstream::in | fstream::out | fstream::binary);
    
    if (!fs)
        throw runtime_error("The metadata file '" + t.name + ".dat' could not open");
}

Metadata::~Metadata()
{
    if (fs.is_open())
        fs.close();
        
    delete[] buffer;
}

int Metadata::nextPageId()
{
    if (pageIdBuffer.empty() && !fs)
        return -1; // we've read last page id.
        
    if (pageIdBuffer.empty()) {
        fillBuffer();
        
        if (pageIdBuffer.empty()) // we can't get a new page id so we're definitely done.
            return -1;
    }
    
    // read from buffer
    int pId = pageIdBuffer.back();
    pageIdBuffer.pop_back();
    return pId;
}

void Metadata::bindNewPage(int pageId)
{
    fs.clear();
    auto pos = fs.tellg();
    fs.seekp(0, ios::end);
    intToBytes(buffer, 0, pageId);
    fs.write(buffer, 4); // 4 = sizeof(int)
    fs.flush();
    fs.clear();
    fs.seekg(pos); // go back where you were
}

// Fill page ID buffer by reading a page of results
void Metadata::fillBuffer()
{
    fs.read(buffer, PAGE_SIZE);
    int numbersRead = fs.gcount() / 4; // 4 is sizeof(int)
    
    for (int i = 0; i < numbersRead; i++)
        pageIdBuffer.push_back(bytesToInt(buffer, i * 4));
}
