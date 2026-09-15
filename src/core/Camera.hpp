class Camera {
public:
    Camera(float viewportWidth, float viewportHeight);

    // Locks directly onto a target center point
    void update(float targetX, float targetY, float mapWidthPx, float mapHeightPx);

    // Smooth follow (lerp factor between 0.0f and 1.0f)
    void follow(float targetX, float targetY, float lerpSpeed, float mapWidthPx, float mapHeightPx);

    float getX() const { return x; }
    float getY() const { return y; }

private:
    float x = 0.0f;
    float y = 0.0f;
    float viewportWidth;
    float viewportHeight;
};