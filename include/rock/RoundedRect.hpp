#pragma once
#include <cocos2d.h>

namespace rock {
    /// @brief Struct representing corner radii for a rounded rectangle
    struct Radii {
        float topLeft;
        float topRight;
        float bottomRight;
        float bottomLeft;

        /// @brief Construct Radii with individual corner radii (clockwise from top-left)
        /// @param tl Top-left radius
        /// @param tr Top-right radius
        /// @param br Bottom-right radius
        /// @param bl Bottom-left radius
        constexpr Radii(float tl = 0.f, float tr = 0.f, float br = 0.f, float bl = 0.f)
            : topLeft(tl), topRight(tr), bottomRight(br), bottomLeft(bl) {}

        /// @brief Construct Radii with uniform corner radius
        /// @param radius Radius for all corners
        static constexpr Radii uniform(float radius) {
            return Radii(radius, radius, radius, radius);
        }
    };

    /// @brief A node similar to CCLayerColor, but with rounded corners
    class RoundedRect : public cocos2d::CCNodeRGBA, public cocos2d::CCBlendProtocol {
    public:
        ~RoundedRect() override;

        /// @brief Create a RoundedRect with specified color, corner radius, and size
        /// @param color Color of the rectangle
        /// @param radius Corner radius for all corners
        /// @param size Size of the rectangle
        static RoundedRect* create(
            cocos2d::ccColor4B color,
            float radius,
            cocos2d::CCSize const& size
        );

        /// @brief Create a RoundedRect with specified color and corner radius, using window size
        /// @param color Color of the rectangle
        /// @param radius Corner radius for all corners
        static RoundedRect* create(
            cocos2d::ccColor4B color,
            float radius
        );

        /// @brief Create a RoundedRect with specified color, corner radii, and size
        /// @param color Color of the rectangle
        /// @param radii Corner radii for each corner
        /// @param size Size of the rectangle
        static RoundedRect* create(
            cocos2d::ccColor4B color,
            Radii const& radii,
            cocos2d::CCSize const& size
        );

        /// @brief Create a RoundedRect with specified color and corner radii, using window size
        /// @param color Color of the rectangle
        /// @param radii Corner radii for each corner
        static RoundedRect* create(
            cocos2d::ccColor4B color,
            Radii const& radii
        );

        /// @brief Set the corner radii
        /// @param radii New corner radii
        void setRadii(Radii const& radii);

        /// @brief Set uniform corner radius for all corners
        /// @param radius New corner radius
        void setRadius(float radius);

        /// @brief Get the current corner radii
        /// @return Current corner radii
        Radii const& getRadii() const;

    protected:
        bool init(
            cocos2d::ccColor4B color,
            Radii const& radii,
            cocos2d::CCSize const& size
        );

        void draw() override;
        void updateColor();
        void updateVertices();

    public:
        cocos2d::ccBlendFunc getBlendFunc() override;
        void setBlendFunc(cocos2d::ccBlendFunc blendFunc) override;

        void setColor(cocos2d::ccColor3B const& color) override;
        void setOpacity(GLubyte opacity) override;
        void setContentSize(cocos2d::CCSize const& contentSize) override;

    protected:
        std::array<cocos2d::ccVertex2F, 4> m_squareVertices{};
        std::array<cocos2d::ccColor4F, 4> m_squareColors{};
        Radii m_radii;
        cocos2d::ccBlendFunc m_blendFunc = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
        GLint m_radiiLoc = -1;
        GLint m_sizeLoc = -1;
    };

    /// @brief A sprite with rounded corners
    class RoundedSprite : public cocos2d::CCSprite {
    public:
        ~RoundedSprite() override;

        /// @brief Create a RoundedSprite with specified image file and corner radii
        /// @param filename Image file path
        /// @param radii Corner radii for each corner
        static RoundedSprite* create(
            char const* filename,
            Radii const& radii
        );

        /// @brief Create a RoundedSprite with specified sprite frame and corner radii
        /// @param spriteFrame Sprite frame to use
        /// @param radii Corner radii for each corner
        static RoundedSprite* createWithSpriteFrame(
            cocos2d::CCSpriteFrame* spriteFrame,
            Radii const& radii
        );

        /// @brief Create a RoundedSprite with specified sprite frame name and corner radii
        /// @param spriteFrameName Name of the sprite frame
        /// @param radii Corner radii for each corner
        static RoundedSprite* createWithSpriteFrameName(
            char const* spriteFrameName,
            Radii const& radii
        );

        /// @brief Create a RoundedSprite with specified texture and corner radii
        /// @param texture Texture to use
        /// @param radii Corner radii for each corner
        static RoundedSprite* createWithTexture(
            cocos2d::CCTexture2D* texture,
            Radii const& radii
        );

        /// @brief Create a RoundedSprite with specified image file and uniform corner radius
        /// @param filename Image file path
        /// @param radius Corner radius for all corners
        static RoundedSprite* create(
            char const* filename,
            float radius
        );

        /// @brief Create a RoundedSprite with specified sprite frame and uniform corner radius
        /// @param spriteFrame Sprite frame to use
        /// @param radius Corner radius for all corners
        static RoundedSprite* createWithSpriteFrame(
            cocos2d::CCSpriteFrame* spriteFrame,
            float radius
        );

        /// @brief Create a RoundedSprite with specified sprite frame name and uniform corner radius
        /// @param spriteFrameName Name of the sprite frame
        /// @param radius Corner radius for all corners
        static RoundedSprite* createWithSpriteFrameName(
            char const* spriteFrameName,
            float radius
        );

        /// @brief Create a RoundedSprite with specified texture and uniform corner radius
        /// @param texture Texture to use
        /// @param radius Corner radius for all corners
        static RoundedSprite* createWithTexture(
            cocos2d::CCTexture2D* texture,
            float radius
        );

    protected:
        bool initWithTexture(
            cocos2d::CCTexture2D* texture,
            Radii const& radii
        );

        bool initWithFile(
            char const* filename,
            Radii const& radii
        );

        bool initWithSpriteFrame(
            cocos2d::CCSpriteFrame* spriteFrame,
            Radii const& radii
        );

        bool initWithSpriteFrameName(
            char const* spriteFrameName,
            Radii const& radii
        );

        bool init(Radii const& radii);
        void draw() override;

    public:
        /// @brief Set the corner radii
        /// @param radii New corner radii
        void setRadii(Radii const& radii);

        /// @brief Set uniform corner radius for all corners
        /// @param radius New corner radius
        void setRadius(float radius);

        /// @brief Get the current corner radii
        /// @return Current corner radii
        Radii const& getRadii() const;

    protected:
        Radii m_radii;
        GLint m_radiiLoc = -1;
        GLint m_sizeLoc = -1;
    };
} // namespace rock