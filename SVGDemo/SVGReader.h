#ifndef SVGREADER_H
#define SVGREADER_H
#include <string>
#include <vector>
class Shape;
class Renderer;
class SVGParser;
class SVGReader {
private:
    std::string m_filePath;
    std::vector<Shape*> m_shapes;
    Renderer* m_renderer;
    SVGParser* m_parser;
    
public:
    bool Load(const std::string& file);
    void Parse();
    void Render();
    std::vector<Shape*> GetShapes();
};
#endif
