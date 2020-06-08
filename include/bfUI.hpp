#pragma once
#ifndef GUICLASSES_H_INCLUDED
#define GUICLASSES_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <SelbaWard/ProgressBar.hpp>
#include <SFML/Audio.hpp>
#include <functional>
#include <utility>
#include <any>

class State;
class Data;
namespace bfUI
{
    class ITransformable {
    public:
        virtual ~ITransformable()=default;
        virtual void setPosition(float x, float y)=0;
        virtual void setPosition(const sf::Vector2f& position)=0;
        virtual void setRotation(float angle)=0;
        virtual void setScale(float factorX, float factorY)=0;
        virtual void setScale(const sf::Vector2f& factors)=0;
        virtual void setOrigin(float x, float y)=0;
        virtual void setOrigin(const sf::Vector2f& origin)=0;
        virtual const sf::Vector2f& getPosition() const=0;
        virtual float getRotation() const=0;
        virtual const sf::Vector2f& getScale() const=0;
        virtual const sf::Vector2f& getOrigin() const=0;
        virtual void move(float offsetX, float offsetY)=0;
        virtual void move(const sf::Vector2f& offset)=0;
        virtual void rotate(float angle)=0;
        virtual void scale(float factorX, float factorY)=0;
        virtual void scale(const sf::Vector2f& factor)=0;
        virtual const sf::Transform& getTransform() const=0;
        virtual const sf::Transform& getInverseTransform() const=0;
    };
	extern sf::Texture uiBackground;
    extern sf::Sound clickSound;
    extern sf::SoundBuffer clickSoundBuffer;
    class Widget {
    protected:
        std::function<void(std::vector<std::any>)> f;
        std::vector<std::any> params;
    public:
        std::string name;
        void setFunction(const std::function<void(std::vector<std::any>)>& func, std::vector<std::any>&& parameters) {f=func;params=parameters;}
        const std::function<void(std::vector<std::any>)>& getFunction() {return f;}
        const std::vector<std::any>& getParameters() {return params;}
        explicit Widget(std::string key, std::function<void(std::vector<std::any>)> func=std::function<void(std::vector<std::any>)>()): f(std::move(func)), name(std::move(key)) {}
        virtual void passEvent(sf::Event event, const sf::Window& window)=0;
        virtual ~Widget()=default;
    };
    class DrawableWidget: public Widget, public sf::Drawable {
    public:
        explicit DrawableWidget(std::string key, std::function<void(std::vector<std::any>)> func=std::function<void(std::vector<std::any>)>()): Widget(std::move(key), std::move(func)) {}
    };
    class WidgetList {
    private:
        bool isPressed;
        const sf::Window* window;
        std::map<std::string, std::unique_ptr<Widget> > widgets;
    public:
        explicit WidgetList(const sf::Window* windowP): window(windowP) {isPressed=false;}
        void passEvent(sf::Event event);
        bool addWidget(std::unique_ptr<Widget> widget);
        bool removeWidget(const std::string& key);
    };
class Button: public DrawableWidget {
protected:
    bool isActive;
    static const Data* data;
public:
    // It should NOT be directly used outside this and MegaTexture
    sf::RectangleShape icon, activeIcon;
    //
    virtual ~Button()=default;
    sf::Color unactiveColor, activeColor;
    explicit Button(const std::string& name="", bool is=true): DrawableWidget(name), isActive(is) {}
    sf::Text text;
    static void setData(const Data& dataR) {data=&dataR;}
    void setActive(bool is){isActive=is;icon.setFillColor(unactiveColor);}
    void updateTextTransform();
    void setText(const sf::String& str, const sf::Font& font=sf::Font(), const sf::Color& color=sf::Color::Black);
    void setCharacterSize(uint32_t size) {text.setCharacterSize(size);}
    bool getActive () const {return isActive;}
    bool wasPressed(const sf::Event& event, const sf::Window& window);
    void passEvent(sf::Event event, const sf::Window& window) override {if(wasPressed(event,window) && f) f(params);}
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {target.draw(icon,states);target.draw(text,states);}
    void setTexture (const sf::Texture& tex, const sf::Texture& activeTex=sf::Texture(), bool resetRect=false);
    void setSize (const sf::Vector2f& v) {setSize(v.x,v.y);}
    void setSize (float x, float y);
    const sf::Vector2f& getSize() const {return icon.getSize();}
    void setScale(const sf::Vector2f& v) {icon.setScale(v.x,v.y);}
    void setScale (float x, float y) {icon.setScale(x,y);activeIcon.setScale(x,y);}
    void setOutlineColor (sf::Color color) {icon.setOutlineColor(color);activeIcon.setOutlineColor(color);}
    void setOutlineThickness (float thickness) {icon.setOutlineThickness(thickness);activeIcon.setOutlineThickness(thickness);}
    void setUnactiveColor() {icon.setFillColor(unactiveColor);activeIcon.setFillColor(unactiveColor);}
    void setActiveColor() {icon.setFillColor(activeColor);activeIcon.setFillColor(activeColor);}
    void setPosition (float x, float y) {icon.setPosition(x,y);activeIcon.setPosition(x,y);}
    void setPosition (const sf::Vector2f &position) {setPosition(position.x,position.y);}
};
class CheckBox: public Button
{
private:
    bool isChecked;
public:
    explicit CheckBox(const std::string& name="", bool is=true, bool ch=false): Button(name, is), isChecked(ch) {}
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {if(!isChecked)target.draw(icon,states);else target.draw(activeIcon,states);target.draw(text,states);}
    bool getChecked() const {return isChecked;}
    void setChecked(bool ch);
    bool checkPressed(const sf::Event& event, const sf::Window& window);
    void passEvent(sf::Event event, const sf::Window& window) override {if(checkPressed(event,window) && f) f(params);}
};

class Slider: public DrawableWidget, public ITransformable
{
    int16_t maxValue, value;
    bool isActive;
    static const Data* data;
    void setTexture (const sf::Texture &texture, bool resetRect=false);
    sw::ProgressBar bar;
public:
    sf::Text text;
    explicit Slider(std::string key=""): DrawableWidget(key) {bar.setColor(sf::Color::White);}
    static void setData(const Data& data) {Slider::data=&data;}
    void initTextures();
    void passEvent(sf::Event event, const sf::Window& window) override {}
    ///Drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    ///Transformable
    void setPosition (float x, float y) override;
    void setPosition (const sf::Vector2f &position) override {setPosition(position.x,position.y);}
    const sf::Transform& getInverseTransform () const override {return bar.getInverseTransform();}
    const sf::Vector2f& getOrigin () const override {return bar.getOrigin();}
    const sf::Vector2f& getPosition () const override {return bar.getPosition();}
    float getRotation () const override {return bar.getRotation();}
    const sf::Vector2f& getScale () const override {return bar.getScale();}
    const sf::Transform& getTransform () const override {return bar.getTransform();}
    void move (float offsetX, float offsetY) override;
    void move (const sf::Vector2f & offset) override {move(offset.x,offset.y);}
    void rotate (float angle) override {setRotation(angle+bar.getRotation());}
    void scale (float factorX, float factorY) override {setScale(bar.getScale().x*factorX,bar.getScale().y*factorY);}
    void scale (const sf::Vector2f& factor) override {scale(factor.x,factor.y);}
    void setOrigin (float x, float y) override;
    void setOrigin (const sf::Vector2f& origin) override {setOrigin(origin.x,origin.y);}
    void setRotation (float angle) override;
    void setScale(float x, float y) override;
    void setScale(const sf::Vector2f &factors) override {setScale(factors.x,factors.y);}
    sf::FloatRect getLocalBounds () const {return bar.getLocalBounds();}
    sf::FloatRect getGlobalBounds () const {return bar.getGlobalBounds();}
    ///Sprite
    const sf::Texture* getTexture () const {return &bar.getTexture();}
    void setTextureRect (const sf::IntRect &rectangle) {bar.setTextureRect(rectangle);}
    void setColor (const sf::Color &color) {bar.setColor(color);}
    sf::Color getColor () const {return bar.getColor();}
    void setBackgroundColor (const sf::Color &color) {bar.setBackgroundColor(color);}
    sf::Color getBackgroundColor () const {return bar.getBackgroundColor();}
    void setFrameColor (const sf::Color &color) {bar.setFrameColor(color);}
    sf::Color getFrameColor () const {return bar.getFrameColor();}
    ///Text
    void setString(const sf::String &string){text.setString(string);}
    void setFont (const sf::Font &font) {text.setFont(font);}
    void setCharacterSize (unsigned int size) {text.setCharacterSize(size);}
    void setTextColor(const sf::Color& color) {text.setFillColor(color);}
    ///Rect
    float getFillPercentage() const {return bar.getPercentage();}
    virtual void setFillPercentage(float per);
    virtual ~Slider()=default;
    Slider(std::string name, const Data& data): DrawableWidget(std::move(name)) {bar.setBackgroundColor(sf::Color::White);}
    bool getActive() const {return isActive;}
    void setActive(bool pr) {isActive=pr;}
    bool moveUpdate(sf::Event event, const sf::Window& window); //true if value was changed
    void setMaxValue(int16_t mVal) {maxValue=mVal;}
    void setValue(int16_t val);
    int16_t getMaxValue() const {return maxValue;}
    int16_t getValue() const {return value;}
};
class InsertText: public sf::Drawable, public sf::Transformable
{
private:
    sf::Text text;
public:
    ///sf::Drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const {target.draw(text,states);}
    ///sf::Transformable
    void setPosition (float x, float y) {text.setPosition(x,y);};
    void setPosition (const sf::Vector2f &position) {text.setPosition(position.x,position.y);}
    const sf::Transform& getInverseTransform () const {return text.getInverseTransform();}
    const sf::Vector2f& getOrigin () const {return text.getOrigin();}
    const sf::Vector2f& getPosition () const {return text.getPosition();}
    float getRotation () const {return text.getRotation();}
    const sf::Vector2f& getScale () const {return text.getScale();}
    const sf::Transform& getTransform () const {return text.getTransform();}
    void move (float offsetX, float offsetY) {text.move(offsetX,offsetY);}
    void move (const sf::Vector2f & offset) {move(offset.x,offset.y);}
    void rotate (float angle) {text.rotate(angle);}
    void scale (float factorX, float factorY) {text.scale(factorX,factorY);}
    void scale (sf::Vector2f& factor) {scale(factor.x,factor.y);}
    void setOrigin (float x, float y) {text.setOrigin(x,y);}
    void setOrigin (sf::Vector2f& origin) {text.setOrigin(origin.x,origin.y);}
    void setRotation (float angle) {text.setRotation(angle);}
    void setScale(float x, float y) {text.setScale(x,y);};
    void setScale(const sf::Vector2f &factors) {text.setScale(factors.x,factors.y);}
    sf::FloatRect getLocalBounds () const {return text.getLocalBounds();}
    sf::FloatRect getGlobalBounds () const {return text.getGlobalBounds();}
    ///TEXT functions
    sf::Vector2f findCharacterPos(std::size_t index) {return text.findCharacterPos(index);}
    unsigned int getCharacterSize() const {return text.getCharacterSize();}
    const sf::Color& getFillColor() const {return text.getFillColor();}
    const sf::Font* getFont() const {return text.getFont();}
    const sf::Color& getOutlineColor() const {return text.getOutlineColor();}
    float getOutlineThickness() const {return text.getOutlineThickness();}
    const sf::String& getString() const {return text.getString();}
    sf::Uint32 getStyle() const {return text.getStyle();}
    void setCharacterSize(unsigned int size) {text.setCharacterSize(size);}
    void setFillColor(const sf::Color& color) {text.setFillColor(color);}
    void setFont(const sf::Font& font) {text.setFont(font);}
    void setOutlineColor(const sf::Color color) {text.setOutlineColor(color);}
    void setOutlineThickness (float thickness) {text.setOutlineThickness(thickness);}
    void setString(const sf::String& string) {text.setString(string);}
    void setStyle(sf::Uint32 style) {text.setStyle(style);}
    #if SFML_VERSION_MINOR>=5
    float getLetterSpacing() const {return text.getLetterSpacing();}
    float getLineSpacing() const {return text.getLineSpacing();}
    void setLetterSpacing(float spacingFactor) {text.setLetterSpacing(spacingFactor);}
    void setLineSpacing(float spacingFactor) {text.setLineSpacing(spacingFactor);}
    #endif
    ///characteristic functions
};
}
#endif // GUICLASSES_H_INCLUDED
