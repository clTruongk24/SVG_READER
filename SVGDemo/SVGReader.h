    #ifndef SVGREADER_H
    #define SVGREADER_H

    #include "SVGParser.h"
    #include "Renderer.h"
    #include "Shape.h"
    #include <string>
    #include <vector>

    class SVGReader {
    private:
        std::string m_filePath;
        std::vector<shape*> m_shapes;
        Renderer* m_renderer;
        SVGParser m_parser;
    
    public:
        SVGReader(const string&);
        SVGReader(const string&, Graphics&);

        virtual ~SVGReader();

        bool Load(const std::string& file);
        void Parse();
        void Render();
        void Render(Graphics&);

        std::vector<shape*> GetShapes() const;
	    //Defs* GetDefs() const;

		const viewBox& getViewBox() const;
    };
    #endif
