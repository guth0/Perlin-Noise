#pragma once
#include <SFML/Graphics.hpp>
#include <deque>

struct Particle
{
public:
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;

    static constexpr uint8_t history_frames_between = 3;        // small effect on performnace (higher = longer & laggier lines)
    uint8_t frames_since_last_history = history_frames_between; // must to start at/above history_frames_between
    static constexpr uint8_t max_history_length = 10;
    std::deque<sf::Vector2f> history;

    sf::Color color; // = sf::Color::Red

    Particle(sf::Vector2f position_) // constructor
        : position{position_}, position_last{position_}, acceleration{0.0f, 0.0f}
    {
        history.push_front(position);
    }

    void update(const float &dt)
    {
        // Compute how much particle moved
        const sf::Vector2f displacement = position - position_last;
        // Update pos

        position_last = position;
        position = position + displacement + acceleration * (dt * dt);

        // Reset acceleration
        acceleration = {};
    }

    void updateHistory()
    {
        if (frames_since_last_history >= history_frames_between)
        {
            history.push_front(position);

            // Limit the history length to control the tail length
            while (history.size() > max_history_length)
            {
                history.pop_back();
            }
            frames_since_last_history = 0;
        }
        else
        {
            frames_since_last_history++;
        }
    }

    void forceUpdateHistory()
    {
        history.push_front(position);

        // Limit the history length to control the tail length
        while (history.size() > max_history_length)
        {
            history.pop_back();
        }
        frames_since_last_history = 0;
    }

    void clearHistory()
    {
        history.clear();
    }

    void accelerate(const sf::Vector2f &a)
    {
        acceleration += a;
    }

    inline void setVelocity(const sf::Vector2f &v, const float &dt)
    {
        position_last = position - (v * dt);
    }

    void ratioSetVelocity(const sf::Vector2f &v, const float &dt, const float &ratio)
    {
        sf::Vector2f pos = ((position - position_last) * (1.0f - ratio) * dt);
        pos += (position - (v * dt)) * ratio;

        position_last = pos;
    }

    inline void addVelocity(const sf::Vector2f &v, const float &dt)
    {
        position_last -= v * dt;
    }

    void slowDown(const float &ratio)
    {
        position_last = position_last + ratio * (position - position_last);
    }

    void setPosition(const float &x, const float &y)
    {
        sf::Vector2f position_ = sf::Vector2f{x, y};
        position_last = position_ - (position - position_last); // Keep momentum
        position = position_;
        forceUpdateHistory();
    }

    [[nodiscard]] sf::Vector2f getVelocity(const float &dt) const
    {
        return (position - position_last) / dt;
    }

    [[nodiscard]] float getSpeed() const
    {
        sf::Vector2f v = position - position_last;
        return sqrt(v.x * v.x + v.y * v.y);
    }
};
