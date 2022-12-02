#include "ofApp.h"
#include "ofApp.h"
#include "ofxGui.h"
#include <glm/gtx/intersect.hpp>
#include <string>
#include <algorithm>    
#include <iomanip>

using namespace std;

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetBackgroundColor(ofColor::black);

    gui.setup();

    theCam = &mainCam;
    mainCam.setDistance(50);
    mainCam.setNearClip(.1);

    sideCam.setPosition(glm::vec3(50, 5, 50));
    sideCam.lookAt(glm::vec3(0, 0, 0));
    sideCam.setNearClip(.1);   

    gui.add(slider_intensity[0].setup("intensity1", 1, 0, 15));
    gui.add(slider_intensity[1].setup("intensity2", 2.5, 0, 15));
    gui.add(slider_intensity[2].setup("intensity3", 1, 0, 15));

    gui.add(power.setup("power", 1000, 10, 10000));
    // floor plane
    imageTextures[0].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Textures\\block\\oak_planks.png");

    // wall planes
    imageTextures[1].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Textures\\block\\cobblestone.png");



    scene.push_back(new Sphere(glm::vec3(0, 0.5, 0), 1.5, ofColor::blue));
    scene.push_back(new Sphere(glm::vec3(0, 1, 3.5), 0.7, ofColor::green));
    scene.push_back(new Sphere(glm::vec3(1.7, 0.5, 1.2), 1.3, ofColor::red));

    // planes

    scene.push_back(new Plane(&imageTextures[0], glm::vec3(0, -2, 0), glm::vec3(0, 1, 0), ofColor::gray)); // floor
    scene.push_back(new Plane(&imageTextures[1], glm::vec3(0, -1, -3), glm::vec3(0, 0, 1), ofColor::gray)); // back wall

    scene.push_back(new Plane(&imageTextures[1], glm::vec3(-6, -1, 0), glm::vec3(1, 0, 0), ofColor::gray)); // left wall
    scene.push_back(new Plane(&imageTextures[1], glm::vec3(6, -1, 0), glm::vec3(-1, 0, 0), ofColor::gray)); // right wall

    for (int i = 0; i < amountOfPlanes; i++)
        scene[scene.size() - i - 1]->hasTexture = true;

    // program is set at a max of three light objects
    light_scene.push_back(new Light(glm::vec3(4, 3, 3), slider_intensity[0])); // to the right
    light_scene.push_back(new Light(glm::vec3(-4, 3, 4), 1.5)); // to the left
    //light_scene.push_back(new Light(glm::vec3(0, 3, 4), slider_intensity[1])); // directly above
    light_scene.push_back(new Light(glm::vec3(0, 1, 8), slider_intensity[2])); // above renderCam

    image.allocate(imageWidth, imageHeight, OF_IMAGE_COLOR);
    image.setColor(ofColor::black);
}

//--------------------------------------------------------------
void ofApp::update() {
    for (int i = 0; i < light_scene.size(); i++)
    {
        light_scene[i]->intensity = slider_intensity[i];
    }
  
}

//--------------------------------------------------------------
void ofApp::draw() {
    theCam->begin();

    renderCam.draw();

    renderCam.view.draw();


    for (SceneObject* s : scene)
    {
        ofSetColor(s->diffuseColor);
        s->draw();
    }

    ofSetColor(ofColor::orangeRed);
    for (Light* l : light_scene)
    {
        l->draw();
    }

    ofSetColor(ofColor::white);

    theCam->end();

    image.draw(0, 0);
    gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    string imageName = "finalImage.jpg";
    string filePath = "C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Final_Images\\" + imageName;
    ifstream ifile;

    switch (key) {
    case OF_KEY_F1:
        theCam = &mainCam;
        break;
    case OF_KEY_F2:
        theCam = &sideCam;
        break;
    case OF_KEY_F3:
        theCam = &previewCam;
        previewCam.setPosition(renderCam.position);
        break;
    case OF_KEY_CONTROL:
        rayTrace();
        break;
    case OF_KEY_TAB:
        image.saveImage(filePath, OF_IMAGE_QUALITY_BEST);
        cout << "Image sucessfully saved to " << filePath << endl;
        break;

        break;

    default:
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}


void ofApp::rayTrace()
{
    cout << "Ray Trace starting    " << endl;

    for (int i = 0; i < imageWidth; i++)
    {
        for (int j = 0; j < imageHeight; j++)
        {
            SceneObject* closest = NULL;
            float closestDist = 9999, currentDist = 0;
            bool hitSurface = false;

            float u = static_cast<float>(i + .5) / static_cast<float>(imageWidth);
            float v = 1.0 - static_cast<float>(j + .5) / static_cast<float>(imageHeight);

            Ray camRayPos = renderCam.getRay(u, v);

            for (int s = 0; s < scene.size(); s++)
            {
                ofColor finalColor = 0;
                glm::vec3 intersectPt, normal;

                hitSurface = scene[s]->intersect(camRayPos, intersectPt, normal);
                if (!hitSurface)
                {
                    continue;
                }

                currentDist = glm::distance(renderCam.position, intersectPt);

                if (currentDist < closestDist || closest == NULL)
                {
                    closestDist = currentDist;
                    closest = scene[s];
                }

                if (hitSurface && (closest == scene[s]))
                {
                    for (Light* lights : light_scene)
                    {
                        glm::vec3 light_pos = lights->position;
                        bool shadowBlocked = false;

                        for (int s2 = 0; s2 < scene.size() - amountOfPlanes; s2++) // loops through scene objects (except the 2 plane objects)
                        {
                            glm::vec3 intersectPt2, normal2;

                            Ray shadRay = Ray(intersectPt + (0.001 * normal), glm::normalize(light_pos - intersectPt));
                            shadowBlocked = scene[s2]->intersect(shadRay, intersectPt2, normal2);

                            ofColor sceneColor = scene[s]->textureLookupColor(intersectPt);
                            finalColor += sceneColor * lights->intensity / 30; // ambience color
                            finalColor += lambert(intersectPt, normal, sceneColor);
                            finalColor += phong(intersectPt, normal, sceneColor, sceneColor, power);

                            if (shadowBlocked && lights->intensity != 0)
                            {
                                finalColor /= SHADOW_DARKENER; // darkens the shadows
                            }
                        }
                    }

                    image.setColor(i, j, finalColor);
                }
            }
        }
    }
    image.update();
    cout << "Ray Trace ending    " << endl;

}

// (x,y,z) -> (x,y)
glm::vec2 Plane::toObjectSpace(glm::vec3 p)
{
    glm::vec2 xy;

    if (normal == glm::vec3(0, 1, 0)) // floor plane
    {
        // xy = x,z
        xy[0] = p[0] - position[0];
        xy[1] = p[2] - position[2];
    }
    else if (normal == glm::vec3(0, 0, 1)) // back wall plane
    {
        // xy = x,y
        xy[0] = p[0] - position[0];
        xy[1] = p[1] - position[1];
    }
    else if ( normal == glm::vec3(1, 0, 0) || normal == (glm::vec3(-1 ,0 ,0))) // side wall plane
    {
        // xy = y,z
        xy[0] = p[1] - position[1];
        xy[1] = p[2] - position[2];
    }
    else
    {
        xy = glm::vec2(-1, -1);
    }

    return xy;
}

// (x,y) -> (u,v)
glm::vec2 Plane::xyScale(glm::vec2 xy)
{
    glm::vec2 uv;
    int numOfTiles = 5;

    uv[0] = ofMap(xy[0], -10, 10, 0, numOfTiles);
    uv[1] = ofMap(xy[1], -10, 10, 0, numOfTiles);

    return uv;

}

// (u,v) -> (i, j)
ofColor Plane::textureLookupColor(glm::vec3 p)
{
    if (!hasTexture)
    {
        return diffuseColor;
    }
    //cout << "in" << endl;

    glm::vec2 xy = toObjectSpace(p);
    glm::vec2 uv = xyScale(xy);

    float bPwidth = imageTexture->getWidth();
    float bPheight = imageTexture->getHeight();

    int i = round((uv[0] * bPwidth) - 0.5);
    int j = round((uv[1] * bPheight) - 0.5);
   
    i = fmod(i, bPwidth);

    j = fmod(j, bPheight);

    return imageTexture->getColor(i, j); 

}

ofColor ofApp::lambert(const glm::vec3& p, const glm::vec3& norm, const ofColor diffuse)
{
    ofColor pixelColor = 0;
    ofColor kd = diffuse;

    for (Light* lights : light_scene)
    {
        glm::vec3 light_pos = lights->position;
        glm::vec3 l = glm::normalize(light_pos - p);

        float intensity = lights->intensity;
        float radius = glm::distance(p, light_pos); 
        float illumination = (intensity) / (radius * radius);

        double nl = glm::dot(l, glm::normalize(norm));

        pixelColor += (kd) * (illumination) * glm::max(0.0, nl);
    }
    return pixelColor;
}

ofColor ofApp::phong(const glm::vec3& p, const glm::vec3& norm, const ofColor diffuse, const ofColor specular, float power)
{
    ofColor pixelColor = 0;
    ofColor phongColor = 0;
    ofColor ks = specular;

    for (Light* lights : light_scene)
    {
        glm::vec3 v = glm::normalize(renderCam.position - p);
        glm::vec3 light_pos = lights->position;

        float intensity = lights->intensity;
        float radius = glm::distance(p, light_pos);
        float illumination = (intensity) / (radius * radius);

        glm::vec3 l = glm::normalize(light_pos - p);
        glm::vec3 h = glm::normalize((v + l));

        double nh = glm::dot(h, glm::normalize(norm));


        pixelColor += ((ks) * (illumination)*glm::pow(glm::max(0.0, nh), power));
    }

    return pixelColor;
}



// Intersect Ray with Plane  (wrapper on glm::intersect*)
//
bool Plane::intersect(const Ray& ray, glm::vec3& point, glm::vec3& normalAtIntersect) {
    float dist;
    bool insidePlane = false;
    bool hit = glm::intersectRayPlane(ray.p, ray.d, position, this->normal, dist);
    if (hit) {
        Ray r = ray;
        point = r.evalPoint(dist);
        normalAtIntersect = this->normal;
        glm::vec2 xrange = glm::vec2(position.x - width / 2, position.x + width / 2);
        glm::vec2 yrange = glm::vec2(position.y - width / 2, position.y + width / 2);
        glm::vec2 zrange = glm::vec2(position.z - height / 2, position.z + height / 2);

        // horizontal 
        //
        if (normal == glm::vec3(0, 1, 0) || normal == glm::vec3(0, -1, 0)) {
            if (point.x < xrange[1] && point.x > xrange[0] && point.z < zrange[1] && point.z > zrange[0]) {
                insidePlane = true;
            }
        }
        // front or back
        //
        else if (normal == glm::vec3(0, 0, 1) || normal == glm::vec3(0, 0, -1)) {
            if (point.x < xrange[1] && point.x > xrange[0] && point.y < yrange[1] && point.y > yrange[0]) {
                insidePlane = true;
            }
        }
        // left or right
        //
        else if (normal == glm::vec3(1, 0, 0) || normal == glm::vec3(-1, 0, 0)) {
            if (point.y < yrange[1] && point.y > yrange[0] && point.z < zrange[1] && point.z > zrange[0]) {
                insidePlane = true;
            }
        }
    }
    return insidePlane;
}



// Convert (u, v) to (x, y, z)
// We assume u,v is in [0, 1]
//
glm::vec3 ViewPlane::toWorld(float u, float v) {
    float w = width();
    float h = height();
    return (glm::vec3((u * w) + min.x, (v * h) + min.y, position.z));
}

// Get a ray from the current camera position to the (u, v) position on
// the ViewPlane
//
Ray RenderCam::getRay(float u, float v) {
    glm::vec3 pointOnPlane = view.toWorld(u, v);
    return(Ray(position, glm::normalize(pointOnPlane - position)));
}