#include "bfUI.hpp"
#include "basic/constants.hpp"
#include "Data.hpp"
#include <cmath>
sf::SoundBuffer bfUI::clickSoundBuffer;
sf::Sound bfUI::clickSound(bfUI::clickSoundBuffer);
const Data* bfUI::Button::data=nullptr;
const Data* bfUI::Slider::data=nullptr;
void bfUI::WidgetList::passEvent(sf::Event event)
{
    //event isn't mouse or touch event
    if (!((event.type == sf::Event::MouseMoved) || (event.type == sf::Event::TouchMoved)
          || (event.type == sf::Event::MouseButtonPressed) || (event.type == sf::Event::TouchBegan)
          || (event.type == sf::Event::MouseButtonReleased) || (event.type == sf::Event::TouchEnded)
          || (event.type == sf::Event::MouseWheelScrolled)))
    {
        isPressed=false;
        return;
    }
    isPressed = (event.type == sf::Event::MouseButtonPressed);
    for (auto it = widgets.begin(); it != widgets.end(); ++it)
        it->second->passEvent(event,*window);
}
bool bfUI::WidgetList::addWidget(std::unique_ptr<Widget> widget)
{
    if(widgets.find(widget->name)!=widgets.end())
        return false;
    widgets[widget->name]=std::move(widget);
    return true;
}
bool bfUI::WidgetList::removeWidget(const std::string& key)
{
    auto it=widgets.find(key);
    if(it==widgets.end())
        return false;
    widgets.erase(it);
    return true;
}
void bfUI::Button::setTexture (const sf::Texture& tex, const sf::Texture& activeTex, bool resetRect)
{
    icon.setTexture(&tex, resetRect);
    if(activeTex.getSize().x>0)
        icon.setTexture(&activeTex);
    else
        icon.setTexture(&tex, resetRect);
}
void bfUI::Button::setSize (float x, float y)
{
    const sf::Texture* iconT=icon.getTexture();
    const sf::Texture* aIconT=activeIcon.getTexture();
    icon=sf::RectangleShape({x,y});
    icon.setTexture(iconT,false);
    activeIcon=sf::RectangleShape({x,y});
    activeIcon.setTexture(aIconT,false);
}

void bfUI::CheckBox::setChecked(bool ch)
{
    isChecked=ch;
}
bool bfUI::CheckBox::checkPressed(const sf::Event& event, const sf::Window& window)
{
    if(wasPressed(event, window))
    {
        //isChecked=!isChecked;
        return true;
    }
    return false;
}
void bfUI::Slider::initTextures()
{
    if(!data)
        throw("Trying to access to nullptr\n");
    bar.setTexture(data->getSliderTexture());
    setTexture(data->getBarTexture());
}
void bfUI::Slider::setValue(int16_t val)
{
    if(!data)
        throw("Trying to access to nullptr\n");
    value=val;
    bar.setPercentage((float)val/(float)maxValue);
}
bool bfUI::Slider::moveUpdate(sf::Event event, const sf::Window& window) {
    if (!isActive) return false;
    my_vector2f relativeMousePosition = sf::Mouse::getPosition(window);
    sf::FloatRect rect = bar.getGlobalBounds();
    if (relativeMousePosition.x > rect.left + rect.width || relativeMousePosition.y > rect.top + rect.height
        || relativeMousePosition.x < rect.left || relativeMousePosition.y < rect.top)
        return false;
    //this->icon.setFillColor(activeColor);
    if (event.type != sf::Event::MouseButtonPressed)
        return false;
    int newValue = std::round(((relativeMousePosition.x - rect.left) / rect.width) * maxValue);
    if (newValue == getValue()) return false;
    setValue(newValue);
    return true;
}
void bfUI::Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(bar,states);
    target.draw(text,states);
}
void bfUI::Slider::setTexture (const sf::Texture &texture, bool resetRect)
{
    bar.setTexture(texture, resetRect);
}
void bfUI::Slider::setPosition (float x, float y)
{
    bar.setPosition(x,y);
    text.setPosition(x+text.getPosition().x-bar.getPosition().x,y+text.getPosition().y-bar.getPosition().y);
}
void bfUI::Slider::move (float offsetX, float offsetY)
{
    bar.move(offsetX,offsetY);
    text.move(offsetX,offsetY);
}
void bfUI::Slider::setOrigin (float x, float y)
{
    bar.setOrigin(x,y);
    text.setOrigin(x,y);
}
void bfUI::Slider::setRotation (float angle)
{
    bar.setRotation(angle);
    sf::Vector2f tmpPos=text.getPosition();
    text.move(tmpPos.x+bar.getPosition().x,-tmpPos.y+bar.getPosition().y);
    text.setRotation(angle);
    text.move(tmpPos.x-bar.getPosition().x,tmpPos.y-bar.getPosition().y);
}
void bfUI::Slider::setScale(float x, float y)
{
    bar.setScale(x,y);
    text.setScale(x,y);
}
void bfUI::Slider::setFillPercentage(float per)
{
    bar.setPercentage(per);
}
bool bfUI::Button::wasPressed(const sf::Event& event, const sf::Window& window)
{
    my_vector2f tmpMousePos=sf::Mouse::getPosition(window);
    if(!isActive) {icon.setFillColor(unactiveColor);return false;}
    sf::FloatRect rect=icon.getGlobalBounds();
    if(tmpMousePos.x>rect.left+rect.width || tmpMousePos.y>rect.top+rect.height
       || tmpMousePos.x<rect.left || tmpMousePos.y<rect.top)
        {this->icon.setFillColor(unactiveColor);return false;}
    this->icon.setFillColor(activeColor);
    if(event.type!=sf::Event::MouseButtonPressed)
        return false;
    if(bfUI::clickSound.getStatus()!=sf::Sound::Status::Playing)
    {
        bfUI::clickSound.play();
    }
    return true;
}
void bfUI::Button::setText(const sf::String& str, const sf::Font& font, const sf::Color& color)
{
    text.setString(str);
    text.setFont(font);
    text.setFillColor(color);
}
void bfUI::Button::updateTextTransform()
{
    sf::Vector2f tmpDiff = sf::Vector2f((icon.getGlobalBounds().width-text.getGlobalBounds().width)*.5f,(icon.getGlobalBounds().height-text.getGlobalBounds().height)*.25f);
    text.setPosition(icon.getPosition()+tmpDiff);
}
