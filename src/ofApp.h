//
//  RayCaster - Set of simple classes to create a camera/view setup for our Ray Tracer HW Project
//
//  I've included these classes as a mini-framework for our introductory ray tracer.
//  You are free to modify/change.   
//
//  These classes provide a simple render camera which can can return a ray starting from
//  it's position to a (u, v) coordinate on the view plane.
//
//  The view plane is where we can locate our photorealistic image we are rendering.
//  The field-of-view of the camera by moving it closer/further 
//  from the view plane.  The viewplane can be also resized.  When ray tracing an image, the aspect
//  ratio of the view plane should the be same as your image. So for example, the current view plane
//  default size is ( 6.0 width by 4.0 height ).   A 1200x800 pixel image would have the same
//  aspect ratio.
//
//  This is not a complete ray tracer - just a set of skelton classes to start.  The current
//  base scene object only stores a value for the diffuse/specular color of the object (defaut is gray).
//  at some point, we will want to replace this with a Material class that contains these (and other 
//  parameters)
//  
//  (c) Kevin M. Smith  - 24 September 2018
//
#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include <glm/gtx/intersect.hpp>

//  General Purpose Ray class 
//
class Ray {
public:
	Ray(glm::vec3 p, glm::vec3 d) { this->p = p; this->d = d; }
	void draw(float t) { ofDrawLine(p, p + t * d); }

	glm::vec3 evalPoint(float t) {
		return (p + t * d);
	}

	glm::vec3 p, d;
};

//  Base class for any renderable object in the scene
//
class SceneObject {
public: 
	virtual void draw() = 0;    // pure virtual funcs - must be overloaded
	virtual bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) { cout << "SceneObject::intersect" << endl; return false; }
	virtual ofColor textureLookupColor(glm::vec3 p, string texture = "") // no textures
	{
		return diffuseColor;
	}	

	// any data common to all scene objects goes here
	glm::vec3 position = glm::vec3(0, 0, 0);

	// material properties (we will ultimately replace this with a Material class - TBD)
	//
	ofColor diffuseColor = ofColor::grey;    // default colors - can be changed.
	ofColor specularColor = ofColor::lightGray;

	bool hasTexture = false;
	ofImage* imageTexture;
	ofImage* imageNormalTexture;
};






//  General purpose sphere  (assume parametric)
//
class Sphere: public SceneObject {
public:
	Sphere(glm::vec3 p, float r, ofColor diffuse = ofColor::lightGray) { position = p; radius = r; diffuseColor = diffuse; }
	Sphere() {}

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
	}
	void draw()  { 
		ofDrawSphere(position, radius); 
	}

	float radius = 1.0;
};

//  Mesh class (will complete later- this will be a refinement of Mesh from Project 1)
//
class Mesh : public SceneObject {
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) { return false;  }
	void draw() { }
};


class Plane : public SceneObject {
public:
	Plane(glm::vec3 p, glm::vec3 n, ofColor diffuse = ofColor::green, ofImage* i = nullptr, ofImage* i_n = nullptr, float w = 20, float h = 20) {
		position = p; normal = n;
		width = w;
		height = h;
		diffuseColor = diffuse;
		imageTexture = i;
		imageNormalTexture = i_n;
		if (normal == glm::vec3(0, 1, 0))
			plane.rotateDeg(-90, 1, 0, 0);
		else if (normal == glm::vec3(0, -1, 0))
			plane.rotateDeg(90, 1, 0, 0);
		else if (normal == glm::vec3(1, 0, 0))
			plane.rotateDeg(90, 0, 1, 0);
		else if (normal == glm::vec3(-1, 0, 0))
			plane.rotateDeg(-90, 0, 1, 0);
	}
	Plane() {
		normal = glm::vec3(0, 1, 0);
		plane.rotateDeg(90, 1, 0, 0);
		//isSelectable = false;
	}

	bool intersect(const Ray& ray, glm::vec3& point, glm::vec3& normal);
	float sdf(const glm::vec3& p);
	glm::vec3 getNormal(const glm::vec3& p) { return this->normal; }
	void draw() {
		plane.setPosition(position);
		plane.setWidth(width);
		plane.setHeight(height);
		plane.setResolution(4, 4);
		plane.drawWireframe();
		//plane.draw();
	}

	// (x,y,z) -> (x,y)
	glm::vec2 toObjectSpace(glm::vec3 p);

	// (x,y) -> (u,v)
	glm::vec2 xyScale(glm::vec2 xy);

	// (u,v) -> (i, j)
	ofColor textureLookupColor(glm::vec3 p, string texture = "");
	

	ofPlanePrimitive plane;
	glm::vec3 normal;

	float width = 20;
	float height = 20;

};


// view plane for render camera
// 
class  ViewPlane: public Plane {
public:
	ViewPlane(glm::vec2 p0, glm::vec2 p1) { min = p0; max = p1; }

	ViewPlane() {                         // create reasonable defaults (6x4 aspect)
		min = glm::vec2(-3, -2);
		max = glm::vec2(3, 2);
		position = glm::vec3(0, 0, 10);
		normal = glm::vec3(0, 0, 1);      // viewplane currently limited to Z axis orientation
	}

	void setSize(glm::vec2 min, glm::vec2 max) { this->min = min; this->max = max; }
	float getAspect() { return width() / height(); }

	glm::vec3 toWorld(float u, float v);   //   (u, v) --> (x, y, z) [ world space ]

	void draw() {
		ofDrawRectangle(glm::vec3(min.x, min.y, position.z), width(), height());
	}

	
	float width() {
		return (max.x - min.x);
	}
	float height() {
		return (max.y - min.y); 
	}

	// some convenience methods for returning the corners
	//
	glm::vec2 topLeft() { return glm::vec2(min.x, max.y); }
	glm::vec2 topRight() { return max; }
	glm::vec2 bottomLeft() { return min;  }
	glm::vec2 bottomRight() { return glm::vec2(max.x, min.y); }

	//  To define an infinite plane, we just need a point and normal.
	//  The ViewPlane is a finite plane so we need to define the boundaries.
	//  We will define this in terms of min, max  in 2D.  
	//  (in local 2D space of the plane)
	//  ultimately, will want to locate the ViewPlane with RenderCam anywhere
	//  in the scene, so it is easier to define the View rectangle in a local'
	//  coordinate system.
	//
	glm::vec2 min, max;
};


//  render camera  - currently must be z axis aligned (we will improve this in project 4)
//
class RenderCam: public SceneObject {
public:
	RenderCam() {
		position = glm::vec3(0, 0, 15);
		aim = glm::vec3(0, 0, -1);
	}
	Ray getRay(float u, float v);
	void draw() { ofDrawBox(position, 1.0); };
	void drawFrustum();

	glm::vec3 aim;
	ViewPlane view;          // The camera viewplane, this is the view that we will render 
};

class Light : public SceneObject {
public:
	Light(glm::vec3 p, float i, float lightDivider = 1, bool areaLightC = false)
	{
		position = p;
		intensity = i / lightDivider;
		lightIntesityDivider = lightDivider;
		selectionSphere = Sphere(p, radius);
		areaLightChild = areaLightC;
	}
	Light() { position = glm::vec3(0,0,0); intensity = 0; areaLightChild = false; }

	void draw() {
		ofDrawSphere(position, radius);
	}
	bool intersect(const Ray& ray, glm::vec3& point, glm::vec3& normal) {
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
	}
	glm::vec3 position;
	float intensity;
	float radius = 0.1;
	float lightIntesityDivider = 1;
	Sphere selectionSphere;
	bool areaLightChild;
};


class AreaLight : public Light {
private:
	float intensityDivider = 1.5;

public:
	int amountOfLights;
	vector <Light*> lightObjects;
	int length, width;
	glm::vec3 normal;
	glm::vec3 center;
	Plane* selectionPlane;

	AreaLight(glm::vec3 p, float i, int l, int w, glm::vec3 n) : Light(p, i, intensityDivider)
	{
		amountOfLights = l * w;
		length = l;
		width = w;
		normal = n;

		center = position;

		if (normal == glm::vec3(0, 1, 0) || normal == glm::vec3(0, -1, 0)) // floor plane
		{
			center.x += (length-1)/2.0;
			center.z += (width-1)/2.0;
		}
		selectionPlane = new Plane(center, normal, ofColor::green, nullptr, nullptr, length - 1, width - 1);

		for (int x = 0; x < length; x++)
		{
			for (int y = 0; y < width; y++)
			{
				glm::vec3 pt;

				if (normal == glm::vec3(0, 1, 0) || normal == glm::vec3(0, -1, 0)) // floor plane
				{
					pt.x = p.x + x;
					pt.y = p.y;
					pt.z = p.z + y;
				}
				else if (normal == glm::vec3(0, 0, 1) || normal == glm::vec3(0, 0, -1)) // back wall plane
				{
					pt.x = p.x + x;
					pt.y = p.y + y;
					pt.z = p.z;
				}
				else if (normal == glm::vec3(1, 0, 0) || normal == glm::vec3(-1, 0, 0)) // side wall plane
				{
					pt.x = p.x;
					pt.y = p.y + x;
					pt.z = p.z + y;
				}
				else
				{
					cout << "Issue creating area light (Incorrect Normal)" << endl;
				}
				
				cout << "Light Object Created" << endl;
				cout << pt << endl;
				lightObjects.push_back(new Light(pt, i, intensityDivider, true));

			}
		}
	}
	AreaLight() : Light() { length = 0; width = 0; amountOfLights = 0; }

	void draw() {
		selectionPlane->draw();
	}


};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void rayTrace();
		void drawGrid();
		void drawAxis(glm::vec3 position);

		ofColor lambert(const glm::vec3& p, const glm::vec3& norm, const ofColor diffuse);
		ofColor phong(const glm::vec3& p, const glm::vec3& norm, const ofColor diffuse, const ofColor specular, float power);
		ofColor textureLookup(glm::vec2 uv);
		glm::vec2 toObjectSpace(glm::vec3 p);
		glm::vec2 xyScale(glm::vec2 p, Plane* pl);

		bool bHide = true;
		bool bShowImage = false;

		ofEasyCam  mainCam;
		ofCamera sideCam;
		ofCamera previewCam;
		ofCamera  *theCam;    // set to current camera either mainCam or sideCam

		// set up one render camera to render image throughn
		//
		RenderCam renderCam;
		ofImage image;

		vector<SceneObject *> scene;
		vector<Light *> light_scene;

		ofxFloatSlider slider_intensity[10];

		ofxFloatSlider power;

		ofxPanel gui;

		ofImage imageTextures[10];
		ofImage imageNormalTextures[10];

		AreaLight* areaLight1;

		// 6:4 ratio
		const int imageWidth = 300; 
		const int imageHeight = 200; 

		const int amountOfPlanes = 4;

		bool selectedLight;
		bool selectedAreaLight;

		int lightNum = 0;

		const float SHADOW_DARKENER = 1.5;
		const float TEXTURE_SHADOW_DARKENER = 1.1;

};
 