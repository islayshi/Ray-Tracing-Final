#include "ofApp.h"
#include "ofxGui.h"
#include <glm/gtx/intersect.hpp>
#include <string>
#include <algorithm>    
#include <iomanip>
#include <glm/gtx/intersect.hpp>

using namespace std;

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetBackgroundColor(ofColor::black);

    gui.setup();

    theCam = &mainCam;
    mainCam.setDistance(50);
    mainCam.setNearClip(.1);

    gui.add(slider_intensity[0].setup("Intensity 1", 0.5, 0, 15));
    gui.add(slider_intensity[1].setup("Intensity 2", 0.25, 0, 15));
    gui.add(slider_intensity[2].setup("Intensity 3", 0.5, 0, 15));
    gui.add(slider_intensity[3].setup("Area Light Intensity", 1.25, 0, 15));


    gui.add(power.setup("power", 1000, 10, 10000));
    // floor plane textures
    // 
    imageTextures[0].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\planks_oak.png");
    imageNormalTextures[0].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\planks_oak_n.png");
    //imageTextures[0].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\brick.png");
    //imageNormalTextures[0].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\brick_n.png");
    //imageTextures[0].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\log_big_oak_top.png");
    //imageNormalTextures[0].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\log_big_oak_top_n.png");

    // side wall planes textures
    // 
    imageTextures[1].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\cobblestone.png");
    imageNormalTextures[1].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\cobblestone_n.png");
    //imageTextures[1].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\bookshelf.png");
    //imageNormalTextures[1].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\bookshelf_n.png");
    //imageTextures[1].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\end_bricks.png");
    //imageNormalTextures[1].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\end_bricks_n.png");
    
    // back wall plane textures
    // 
    imageTextures[2].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\diamond_block.png");
    imageNormalTextures[2].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\diamond_block_n.png");
    //imageTextures[2].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\bookshelf.png");
    //imageNormalTextures[2].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\bookshelf_n.png");
    //imageTextures[2].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\prismarine_dark.png");
    //imageNormalTextures[2].load("C:\\Users\\ngjwo\\Documents\\of_v0.11.2_vs2017_release\\apps\\myApps\\RayTracingFinal\\Minecraft_Bump_Textures\\blocks\\prismarine_dark_n.png");
    scene.push_back(new Sphere(glm::vec3(0, 0.5, 0), 1.5, ofColor::blue));
    scene.push_back(new Sphere(glm::vec3(0, 1, 3.5), 0.7, ofColor::green));
    scene.push_back(new Sphere(glm::vec3(1.7, 0.5, 1.2), 1.3, ofColor::red));

    // planes

    Plane* floor_plane = new Plane(glm::vec3(0, -2, 0), glm::vec3(0, 1, 0), ofColor::gray, &imageTextures[0], &imageNormalTextures[0]);
    Plane* left_wall_plane = new Plane(glm::vec3(-6, -1, 0), glm::vec3(1, 0, 0), ofColor::gray, &imageTextures[1], &imageNormalTextures[1]);
    Plane* right_wall_plane = new Plane(glm::vec3(6, -1, 0), glm::vec3(-1, 0, 0), ofColor::gray, &imageTextures[1], &imageNormalTextures[1]);
    Plane* back_plane = new Plane(glm::vec3(0, -1, -3), glm::vec3(0, 0, 1), ofColor::gray, &imageTextures[2], &imageNormalTextures[2]);

    scene.push_back(floor_plane); 
    scene.push_back(back_plane); 
    scene.push_back(left_wall_plane); 
    scene.push_back(right_wall_plane);

    for (int i = 0; i < amountOfPlanes; i++)
        scene[scene.size() - i - 1]->hasTexture = true;

    light_scene.push_back(new Light(glm::vec3(4, 3, 3), slider_intensity[0], 1.75)); // to the right
    light_scene.push_back(new Light(glm::vec3(0.5, 4.5, -1.5), slider_intensity[1], 1.75)); // in the back
    light_scene.push_back(new Light(glm::vec3(-4, 3, 4), slider_intensity[2], 1.75)); // in the back

    //light_scene.push_back(new Light(glm::vec3(-4, 3, 4), 1.5)); // to the left
    //light_scene.push_back(new Light(glm::vec3(0, 3, 4), slider_intensity[1])); // directly above
    //light_scene.push_back(new Light(glm::vec3(0, 1, 8), slider_intensity[2])); // above renderCam

  
    areaLight1 = new AreaLight(glm::vec3(2, 12, 4), slider_intensity[3], 2, 4, glm::vec3(0, -1, 0));


    for (int i = 0; i < areaLight1->amountOfLights; i++)
    {
        light_scene.push_back(areaLight1->lightObjects[i]);
    }

    image.allocate(imageWidth, imageHeight, OF_IMAGE_COLOR);
    image.setColor(ofColor::black);
}

//--------------------------------------------------------------
void ofApp::update() {
    int light_counter = 0;
    for (int i = 0; i < light_scene.size(); i++)
    {
        light_scene[i]->intensity = slider_intensity[light_counter] / light_scene[i]->lightIntesityDivider;
        if (!light_scene[i]->areaLightChild)
            light_counter += 1;
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


    ofSetColor(ofColor::cyan);
    areaLight1->draw();

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
    case OF_KEY_F3:
        theCam = &previewCam;
        previewCam.setPosition(renderCam.position);
        break;
    case 'r':
        rayTrace();
        break;
    case 's':
        image.saveImage(filePath, OF_IMAGE_QUALITY_BEST);
        cout << "Image sucessfully saved to " << filePath << endl;
        break;
    case 'c':
        if (mainCam.getMouseInputEnabled()) mainCam.disableMouseInput();
        else mainCam.enableMouseInput();
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
    if (button == 0 && (selectedLight || selectedAreaLight)) // left click
    {
        glm::vec3 zaxis = theCam->getZAxis();

        glm::vec3 screen3DPt = theCam->screenToWorld(glm::vec3(x, y, 0));
        glm::vec3 rayOrigin = theCam->getPosition();

        Ray r = Ray(rayOrigin, glm::normalize(screen3DPt - rayOrigin));
        // if user tries to select both at the same time, only first light object is moved
        if (selectedLight)
        {
            glm::vec3 releasedPosition = r.evalPoint(distance(rayOrigin, light_scene[lightNum]->position));
            cout << "Light" << lightNum << " moved to " << releasedPosition << endl;
            light_scene[lightNum]->position = releasedPosition;
            light_scene[lightNum]->selectionSphere.position = releasedPosition;
            draw();
        }
        else if (selectedAreaLight)
        {
            glm::vec3 releasedPosition = r.evalPoint(distance(rayOrigin, light_scene[lightNum]->position));
            glm::vec3 translate = releasedPosition - light_scene[lightNum]->position;
            cout << "Area Light" << " moved to " << releasedPosition << endl;

            for (Light* lights : light_scene)
            {
                if (lights->areaLightChild)
                {
                    lights->position += translate;
                    lights->selectionSphere.position += translate;
                }
                
            }

            areaLight1->selectionPlane->position += translate;
            draw();
        }

    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    if (button == 0) // left click
    {
        glm::vec3 screen3DPt = theCam->screenToWorld(glm::vec3(x, y, 0));
        glm::vec3 rayOrigin = theCam->getPosition();
        glm::vec3 rayDir = glm::normalize(screen3DPt - rayOrigin);
        glm::vec3 intersectNormal, intersectPt;
        float intersectDistance = 0;

        int counter = 0;
        for (Light* lights : light_scene)
        {
            bool lightIntersect = glm::intersectRaySphere(rayOrigin, rayDir, lights->selectionSphere.position, lights->selectionSphere.radius,
                intersectPt, intersectNormal);

            if (lightIntersect and !lights->areaLightChild) {
                cout << "hit light " << counter << endl;
                selectedLight = true;
                lightNum = counter;
                break;
            }
            else if (lightIntersect and lights->areaLightChild) {
                cout << "hit area light " << endl;
                selectedAreaLight = true;
                lightNum = counter;
                break;
            }
            else {
                selectedLight = false;
                selectedAreaLight = false;
            }
            counter++;
        }

    }

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
                glm::vec3 intersectPt, normal, textureNormal;

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

                        for (int s2 = 0; s2 < scene.size(); s2++) // loops through scene objects 
                        {
                            glm::vec3 intersectPt2, normal2;

                            if (s >= scene.size() - amountOfPlanes) // if it's a plane
                            {
                                if (scene[s]->imageNormalTexture != nullptr) // calculation for RGB normal mapping
                                {
                                    ofColor normalTextureColor = scene[s]->textureLookupColor(intersectPt, "n");
                                    float red, green, blue;

                                    red = ofMap(normalTextureColor.r, 0, 255, -1, 1);
                                    green = ofMap(normalTextureColor.g, 0, 255, -1, 1);
                                    blue = ofMap(normalTextureColor.b, 128, 255, 0, -1);

                                    normal += glm::vec3(red, green, -blue);
                                    normal = glm::normalize(normal);
                                }
                            }

                            Ray shadRay = Ray(intersectPt + (0.001 * normal), glm::normalize(light_pos - intersectPt));
                            shadowBlocked = scene[s2]->intersect(shadRay, intersectPt2, normal2);

                            ofColor sceneColor = scene[s]->textureLookupColor(intersectPt);
                            finalColor += sceneColor * lights->intensity / 50; // ambience color
                            finalColor += lambert(intersectPt, normal, sceneColor);
                            finalColor += phong(intersectPt, normal, sceneColor, sceneColor, power);

                            if (shadowBlocked && lights->intensity != 0 && s < scene.size() - amountOfPlanes)
                            {
                                finalColor /= SHADOW_DARKENER; // darkens the shadows
                            }
                            else if (shadowBlocked && lights->intensity != 0 && s >= scene.size() - amountOfPlanes)
                            {
                                finalColor /= TEXTURE_SHADOW_DARKENER; // darkens the texture shadows
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

    if (normal == glm::vec3(0, 1, 0) || normal == (glm::vec3(0, -1, 0))) // floor plane
    {
        // xy = x,z
        xy[0] = p[0] - position[0];
        xy[1] = p[2] - position[2];
    }
    else if (normal == glm::vec3(0, 0, 1) || normal == (glm::vec3(0, 0, -1))) // back wall plane
    {
        // xy = x,y
        xy[0] = p[0] - position[0];
        xy[1] = p[1] - position[1];
    }
    else if ( normal == glm::vec3(1, 0, 0) || normal == (glm::vec3(-1 ,0 ,0))) // side wall plane
    {
        // xy = y,z
        xy[0] = p[2] - position[2];
        xy[1] = p[1] - position[1];
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
ofColor Plane::textureLookupColor(glm::vec3 p, string texture)
{
    ofImage* iTexture = imageTexture;

    if (texture == "n")
        iTexture = imageNormalTexture;

    if (!hasTexture)
    {
        return diffuseColor;
    }

    glm::vec2 xy = toObjectSpace(p);
    glm::vec2 uv = xyScale(xy);

    float bPwidth = iTexture->getWidth();
    float bPheight = iTexture->getHeight();

    int i = round((uv[0] * bPwidth) - 0.5);
    int j = round((uv[1] * bPheight) - 0.5);
   
    i = fmod(i, bPwidth);

    j = fmod(j, bPheight);

    return iTexture->getColor(i, j);

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



bool Plane::intersect(const Ray& ray, glm::vec3& point, glm::vec3&
    normalAtIntersect) {
    float dist;
    bool insidePlane = false;
    bool hit = glm::intersectRayPlane(ray.p, ray.d, position, this->normal,
        dist);
    if (hit) {
        Ray r = ray;
        point = r.evalPoint(dist);
        normalAtIntersect = this->normal;
        glm::vec2 xrange = glm::vec2(position.x - width / 2, position.x + width
            / 2);
        glm::vec2 zrange = glm::vec2(position.z - height / 2, position.z +
            height / 2);
        if (point.x < xrange[1] && point.x > xrange[0] && point.z < zrange[1]
            && point.z > zrange[0]) {
            insidePlane = true;
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