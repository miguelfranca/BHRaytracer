#pragma once

#include <SFML/Graphics.hpp>

#include "Spacetime.hpp"
#include "Matrix.hpp"
#include "Tools.hpp"

#define BLACK           sf::Color(0,0,0)
#define WHITE           sf::Color(255,255,255)

#define DARK_RED        sf::Color(204, 0, 0)
#define LIGHT_RED       sf::Color(255, 102, 102)
#define LIGHT_ORANGE    sf::Color(255, 153, 51)
#define YELLOW          sf::Color(255, 255, 5)
#define DARK_BLUE       sf::Color(0, 0, 255)
#define LIGHT_BLUE      sf::Color(102, 178, 255)
#define DARK_GREEN      sf::Color(0, 153, 0)
#define LIGHT_GREEN     sf::Color(102, 255, 102)

class SkyMap
{
public:
    SkyMap(const Spacetime& a_st, bool do_elliptic_parametrization);

    virtual sf::Color getPixelColor(double x, double y) = 0;

    sf::Image getSkyView(const Matrix<VecD>& mat);

    VecD getCoordinates(double theta, double phi);

protected:
    const Spacetime& st;
    bool do_elliptic_parametrization = false;

protected:
    static sf::Color mixColors(sf::Color color1, sf::Color color2, double percentage);
};

class ColorSkyMap : public SkyMap
{
public:
    ColorSkyMap(const Spacetime& a_st, bool do_elliptic_parametrization, double grid_angle,
                double thickness, double color_ring = 0.);

    sf::Color getPixelColor(double x, double y);

private:
    double grid_angle;
    double thickness;
    double color_ring;
};

class ImageSkyMap : public SkyMap
{
public:
    ImageSkyMap(const Spacetime& a_st, const std::string& filename, bool do_elliptic_parametrization);
    sf::Color getPixelColor(double x, double y);

private:
    sf::Image sky;
};

sf::Color redshiftColor(sf::Color color, double factor);
sf::Color wavelengthToColor(double wavelength);
