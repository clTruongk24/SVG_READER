# SVG Reader Project

A C++ application for parsing and rendering SVG files using GDI+.

## UML Diagram

<img width="1280" height="720" alt="image" src="https://github.com/user-attachments/assets/0b5e56a7-f589-4c90-b383-7e6080783267" />

## Architecture & Design Patterns

The project follows a modular architecture, separating the concerns of parsing, modeling, and rendering SVG elements.

### 🏛️ Overall Architecture
1.  **Parsing Layer**: Utilizes `rapidxml` to traverse the SVG XML structure. The `SVGParser` class orchestrates the reading of nodes and extraction of attributes.
2.  **Model Layer**: A rich class hierarchy representing SVG elements (Shapes, Gradients, Transforms).
3.  **Rendering Layer**: Uses the Windows GDI+ library to draw the model objects onto a graphics context.

---

### 🎨 Design Patterns Applied

#### 1. Factory Method Pattern
Used in the `factory` class to centralize the creation of SVG shapes.
- **Problem**: The parser doesn't know in advance which specific shape (Circle, Path, Rectangle, etc.) it will encounter.
- **Solution**: `factory::createShape` takes a tag name and attributes, returning a generic `shape*` pointer to a specific subclass.

#### 2. Composite Pattern
Implemented via the `group` class.
- **Problem**: SVG elements can be grouped (`<g>`) and groups can contain other shapes or nested groups.
- **Solution**: The `group` class inherits from `shape` but also contains a collection of `shape*` children, allowing uniform treatment of individual shapes and groups of shapes.

#### 3. Singleton Pattern
Applied to the `Defs` class.
- **Problem**: Gradients and styles defined in the `<defs>` section need to be accessible globally during the parsing and rendering of various shapes.
- **Solution**: `Defs::getInstance()` ensures a single global registry for lookups by ID.

#### 4. Polymorphism (Strategy/Interface)
The `shape` abstract base class defines a standard interface (`draw`, `getBounds`).
- **Benefit**: The renderer can iterate through a list of `shape*` objects and call `draw()` without needing to know the concrete type of each shape.

---

## Technical Stack
- **Language**: C++
- **Graphics Library**: GDI+
- **XML Parser**: RapidXML
- **IDE**: Visual Studio
