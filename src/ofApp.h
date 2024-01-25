//An Tran #014728649
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
	void draw(float t) {
		ofSetColor(ofColor::white);
		ofDrawLine(p, p + t * d);
	}

	void draw2(float t) {
		ofSetColor(ofColor::aqua);
		ofDrawLine(p, p + t * d);
	}

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
	virtual bool intersect(const Ray& ray, glm::vec3& point, glm::vec3& normal) { /*cout << "SceneObject::intersect" << endl;*/ return false; }
	virtual ofColor diffuseTextureLookup(glm::vec3 point) { return diffuseColor; }
	virtual ofColor specularTextureLookup(glm::vec3 point) { return specularColor; }

	/*****************************************************Bump mapping impelementation**************************************************************/
	/*******************************************************************************************************************/
	virtual ofColor bumpTextureLookup(glm::vec3 point) { return bumpColor; }
	virtual ofColor bumpright(glm::vec3 point) { return bumpColor; }
	virtual ofColor bumpdown(glm::vec3 point) { return bumpColor; }

	/*****************************************************End of Bump mapping impelementation**************************************************************/
	/*******************************************************************************************************************/

	/*********************************************************Displacement mapping implementation**********************************************************/
	/*******************************************************************************************************************/
	virtual void generateDistanceMap(glm::vec3 point) { return; }
	virtual bool intersectDistanceMap(const Ray& ray, glm::vec3& point, glm::vec3& normal) { return false; }

	/*********************************************************End of Displacement mapping implementation**********************************************************/
	/*******************************************************************************************************************/

	void setDiffuse(ofColor c) {
		diffuseColor = c;
	}

	virtual void setPosition(glm::vec3 newPos) {
		position = newPos;
	}

	// any data common to all scene objects goes here
	glm::vec3 position = glm::vec3(0, 0, 0);

	// material properties (we will ultimately replace this with a Material class - TBD)
	ofColor diffuseColor = ofColor::grey;    // default colors - can be changed.
	ofColor specularColor = ofColor::lightGray;
	ofColor bumpColor = NULL;
	ofColor dispColor = NULL;
	bool isSelectable;
	bool bTexture = false;
};

//  General purpose sphere  (assume parametric)
//
class Sphere : public SceneObject {
public:
	Sphere(ofImage* bump, ofImage* specular, ofImage* texture, glm::vec3 p, float r, ofColor diffuse = ofColor::lightGray) { this->bump = bump;  this->specular = specular;  this->texture = texture;  position = p; radius = r; diffuseColor = diffuse; }
	Sphere() {}
	bool intersect(const Ray& ray, glm::vec3& point, glm::vec3& normal) {
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
	}


	void generateDistanceMap(ofImage displacementMap) { 
		//
		return; 
	}


	void draw() {
		ofDrawSphere(position, radius);
	}

	/**************************************Bump Map Complete*******************************************/
	/**************************************************************************************************/
	ofColor bumpTextureLookup(glm::vec3 point) {
		if (bump == NULL) {
			return bumpColor;
		}
		glm::vec3 r = glm::normalize(point - position);
		float u = 0.5f + (atan2(r.x, r.z) / 2 * PI);
		float v = 0.5f + (asin(r.y) / PI);
		float i = u * bump->getWidth() - 0.5;
		float j = v * bump->getHeight() - 0.5;
		float ipixel = fmod(i, bump->getWidth());
		float jpixel = fmod(j, bump->getHeight());
		return bump->getColor(ipixel, jpixel);
	}

	/**************************************End of Bump Map Complete*******************************************/
	/**************************************************************************************************/


	/*****************************Bump Map Prototype********************************/
	/*******************************************************************************/
	ofColor bumpright(glm::vec3 point) {	// Grabbing the pixel to the right of the specified pixel
		if (bump == NULL) {
			return bumpColor;
		}
		glm::vec3 r = glm::normalize(point - position);
		float u = 0.5f + (atan2(r.x, r.z) / 2 * PI);
		float v = 0.5f + (asin(r.y) / PI);
		float i = u * bump->getWidth() - 0.5;
		float j = v * bump->getHeight() - 0.5;
		float ipixel = fmod(i, bump->getWidth()) + 1;
		float jpixel = fmod(j, bump->getHeight());
		return bump->getColor(ipixel, jpixel);
	}

	ofColor bumpdown(glm::vec3 point) {		// Grabbing the pixel below the specified pixel	
		if (bump == NULL) {
			return bumpColor;
		}
		glm::vec3 r = glm::normalize(point - position);
		float u = 0.5f + (atan2(r.x, r.z) / 2 * PI);
		float v = 0.5f + (asin(r.y) / PI);
		float i = u * bump->getWidth() - 0.5;
		float j = v * bump->getHeight() - 0.5;
		float ipixel = fmod(i, bump->getWidth());
		float jpixel = fmod(j, bump->getHeight()) - 1;
		return bump->getColor(ipixel, jpixel);
	}

	/*****************************End of Bump Map Prototype********************************/
	/**************************************************************************************/

	ofColor diffuseTextureLookup(glm::vec3 point) {
		if (texture == NULL) {
			return diffuseColor;
		}
		glm::vec3 r = glm::normalize(point - position);
		float u = 0.5f + (atan2(r.x, r.z) / 2 * PI);
		float v = 0.5f + (asin(r.y) / PI);
		float i = u * texture->getWidth() - 0.5;
		float j = v * texture->getHeight() - 0.5;
		float ipixel = fmod(i, texture->getWidth());
		float jpixel = fmod(j, texture->getHeight());
		return texture->getColor(ipixel, jpixel);
	}

	ofColor specularTextureLookup(glm::vec3 point) {
		if (specular == NULL) {
			return specularColor;
		}
		glm::vec3 r = glm::normalize(point - position);
		float u = 0.5f + (atan2(r.x, r.z) / 2 * PI);
		float v = 0.5f + (asin(r.y) / PI);
		float i = u * specular->getWidth() - 0.5;
		float j = v * specular->getHeight() - 0.5;
		float ipixel = fmod(i, specular->getWidth());
		float jpixel = fmod(j, specular->getHeight());
		return specular->getColor(ipixel, jpixel);
	}

	ofImage* texture;
	ofImage* specular;
	ofImage* bump;
	float radius = 1.0;
};

//Light class
//
class Light : public SceneObject {
public:

	Light() {}
	Light(glm::vec3 p, float r, float i) {
		position = p;
		radius = r;
		intensity = i;
	}

	bool intersect(glm::vec3 rayOrigin, glm::vec3 rayDir, Light* l, glm::vec3 intersectNormal, glm::vec3 intersectPt) {
		return glm::intersectRaySphere(rayOrigin, rayDir, l->position, l->radius, intersectPt, intersectNormal);
	}

	virtual bool intersectAim(glm::vec3 rayOrigin, glm::vec3 rayDir, Light* l, glm::vec3 intersectNormal, glm::vec3 intersectPt) {
		return false;
	}

	virtual float lightangle() { 
		return 10000; 
	}
	void setPosition(glm::vec3 newPos) {
		position = newPos;
	}

	virtual void increaseAngle() {};

	virtual void decreaseAngle() {};

	virtual void setAim(glm::vec3 newAim) {};


	void draw() {
		ofDrawSphere(position, radius);
	}

	float radius = 1.0;
	float intensity = 1.0;
	glm::vec3 aimPos = glm::vec3(0, 0, 0);
};

class SpotLight : public Light {
public:
	SpotLight() {}
	SpotLight(glm::vec3 p, glm::vec3 aim, float r, float i, float c) {
		position = p;
		radius = r;
		intensity = i;
		coneWidth = c;
		aimPos = aim;
	}

	/*bool intersect(glm::vec3 rayOrigin, glm::vec3 rayDir, SpotLight* l, glm::vec3 intersectNormal, glm::vec3 intersectPt) {
		return glm::intersectRaySphere(rayOrigin, rayDir, l->position, l->radius, intersectPt, intersectNormal);
	}*/

	bool intersectAim(glm::vec3 rayOrigin, glm::vec3 rayDir, Light* l, glm::vec3 intersectNormal, glm::vec3 intersectPt) {
		return glm::intersectRaySphere(rayOrigin, rayDir, l->aimPos, l->radius, intersectPt, intersectNormal);
	}

	void draw() {
		ofPushMatrix();
		glm::mat4 m = glm::lookAt(position, aimPos, upVector);
		ofMultMatrix(glm::inverse(m));
		ofRotate(-90, 1, 0, 0);
		//ofSetColor(ofColor::lightGray);
		ofDrawCone(coneWidth, 1);
		ofPopMatrix();
		//ofSetColor(ofColor::aquamarine);
		ofDrawSphere(position, radius);
		ofDrawSphere(aimPos, radius);
		//ofSetColor(ofColor::blueSteel);
		ofDrawLine(position, aimPos);
	}

	float lightangle() {
		return 2 * (atan((coneWidth / 2) / coneHeight));
	}

	void setPosition(glm::vec3 newPos) {
		position = newPos;
	}

	void increaseAngle() {
		coneWidth += .1;
	}

	void decreaseAngle() {
		coneWidth -= .1;
	}

	void setAim(glm::vec3 newAim) {
		aimPos = newAim;
	}

	//glm::vec3 aimPos = glm::vec3(0, 0, 0);
	glm::vec3 upVector = glm::vec3(0, 1, 0);
	float coneWidth = 1;
	float coneHeight = 1;

};

//  Mesh class (will complete later- this will be a refinement of Mesh from Project 1)
//
class Mesh : public SceneObject {
	bool intersect(const Ray& ray, glm::vec3& point, glm::vec3& normal) { return false; }
	void draw() { }
};


//  General purpose plane 
//
class Plane : public SceneObject {
public:
	Plane(ofImage* displ, ofImage* bump, ofImage* specular, ofImage* texture, glm::vec3 p, glm::vec3 n, ofColor diffuse = ofColor::green, float w =
		20, float h = 20) {
		position = p; normal = n;
		width = w;
		height = h;
		diffuseColor = diffuse;
		bTexture = true;
		this->texture = texture;
		this->specular = specular;
		this->bump = bump;

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
		isSelectable = false;
		bTexture = true;
	}


	/******************************************Attempt at Displacement mapping*********************************************/
	/**********************************************************************************************************************/


	/*
	* The idea was similar to bump mapping where a value would be obtained from the gray scale image based on its surrounding pixels.
	* These values would be values of the height map.
	* The values would then somehow be saved. This could be a 2d array, or something similar.
	* The ray in question could be run through the saved values, checking if they were to be intersected.
	* Something this solution would also need is the ability to get the pixel above and below.
	* Increasing x by 1 and decreasing the y by 1 in this case is not accurate enough.
	*/
	void generateDistanceMap(glm::vec3 point) {
		if (bump == NULL) {
			return;
		}
		if (normal == glm::vec3(0, 1, 0)) {
			glm::vec2 xy = glm::vec2(point.x - position.x, point.z - position.z);
			glm::vec2 xy2 = glm::vec2((point.x - position.x) + 1, point.z - position.z);
			glm::vec2 xy3 = glm::vec2(point.x - position.x, (point.z - position.z)-1);
			float u = ofMap(xy.x, -10, 10, 0, 4);
			float v = ofMap(xy.y, -10, 10, 0, 4);
			float u2 = ofMap(xy2.x, -10, 10, 0, 4);
			float v2 = ofMap(xy2.y, -10, 10, 0, 4);
			float u3 = ofMap(xy3.x, -10, 10, 0, 4);
			float v3 = ofMap(xy3.y, -10, 10, 0, 4);
			float i = u * bump->getWidth() - 0.5;
			float j = v * bump->getHeight() - 0.5;
			float i2 = u2 * bump->getWidth() - 0.5;
			float j2 = v2 * bump->getHeight() - 0.5;
			float i3 = u3 * bump->getWidth() - 0.5;
			float j3 = v3 * bump->getHeight() - 0.5;
			float ipixel = fmod(i, bump->getWidth());
			float jpixel = fmod(j, bump->getHeight());
			float ipixel2 = fmod(i2, bump->getWidth());
			float jpixel2 = fmod(j2, bump->getHeight());
			float ipixel3 = fmod(i2, bump->getWidth());
			float jpixel3 = fmod(j2, bump->getHeight());

			ofColor right = bump->getColor(ipixel2, jpixel2);
			ofColor down = bump->getColor(ipixel3, jpixel3);
			ofColor pos = bump->getColor(ipixel, jpixel);

			ofColor Bu = pos - right;
			ofColor Bv = pos - down;

			return;
		}
	}

	/*
	* This function would be running the ray in question through the height map.
	* Displacement mapping requires a recursive function which would regenerate a new ray based on
	* the distance of the cloest point of the height map.
	* This would require some complex math to check all the points of the height map and ensure the ray
	* intersect an elevated position of the object and the not the base, undisplaced, object.
	*/
	bool intersectDistanceMap(const Ray& ray, glm::vec3& point, glm::vec3& normal) {
		return false; 
	}

	/**********************************************************************************************************************/
	/**********************************************************************************************************************/

	ofColor bumpTextureLookup(glm::vec3 point) {
		if (bump == NULL) {
			return bumpColor;
		}
		if (normal == glm::vec3(0, 1, 0)) {
			glm::vec2 xy = glm::vec2(point.x - position.x, point.z - position.z);
			float u = ofMap(xy.x, -10, 10, 0, 4);
			float v = ofMap(xy.y, -10, 10, 0, 4);
			float i = u * bump->getWidth() - 0.5;
			float j = v * bump->getHeight() - 0.5;
			float ipixel = fmod(i, bump->getWidth());
			float jpixel = fmod(j, bump->getHeight());
			return bump->getColor(ipixel, jpixel);
		}
		else if (normal == glm::vec3(0, 0, 1)) {
			glm::vec2 xy = glm::vec2(point.x - position.x, point.y - position.y);
			float u = ofMap(xy.x, -10, 10, 0, 1);
			float v = ofMap(xy.y, -10, 10, 0, 1);
			float i = u * bump->getWidth() - 0.5;
			float j = v * bump->getHeight() - 0.5;
			float ipixel = fmod(i, bump->getWidth());
			float jpixel = fmod(j, bump->getHeight());
			return bump->getColor(ipixel, jpixel);
		}
	}

	ofColor diffuseTextureLookup(glm::vec3 point) {
		if (texture == NULL) {
			return diffuseColor;
		}
		if (normal == glm::vec3(0, 1, 0)) {
			glm::vec2 xy = glm::vec2(point.x - position.x, point.z - position.z);
			float u = ofMap(xy.x, -10, 10, 0, 1);
			float v = ofMap(xy.y, -10, 10, 0, 1);
			float i = u * texture->getWidth() - 0.5;
			float j = v * texture->getHeight() - 0.5;
			float ipixel = fmod(i, texture->getWidth());
			float jpixel = fmod(j, texture->getHeight());
			return texture->getColor(ipixel, jpixel);
		}
		else if (normal == glm::vec3(0, 0, 1)) {
			glm::vec2 xy = glm::vec2(point.x - position.x, point.y - position.y);
			float u = ofMap(xy.x, -10, 10, 0, 1);
			float v = ofMap(xy.y, -10, 10, 0, 1);
			float i = u * texture->getWidth() - 0.5;
			float j = v * texture->getHeight() - 0.5;
			float ipixel = fmod(i, texture->getWidth());
			float jpixel = fmod(j, texture->getHeight());
			return texture->getColor(ipixel, jpixel);
		}
		else {
			return diffuseColor;
		}
	}

	ofColor specularTextureLookup(glm::vec3 point) {
		if (specular == NULL) {
			return specularColor;
		}
		if (normal == glm::vec3(0, 1, 0)) {
			glm::vec2 xy = glm::vec2(point.x - position.x, point.z - position.z);
			float u = ofMap(xy.x, -10, 10, 0, 1);
			float v = ofMap(xy.y, -10, 10, 0, 1);
			float i = u * specular->getWidth() - 0.5;
			float j = v * specular->getHeight() - 0.5;
			float ipixel = fmod(i, specular->getWidth());
			float jpixel = fmod(j, specular->getHeight());
			return specular->getColor(ipixel, jpixel);
		}
		else if (normal == glm::vec3(0, 0, 1)) {
			glm::vec2 xy = glm::vec2(point.x - position.x, point.y - position.y);
			float u = ofMap(xy.x, -10, 10, 0, 1);
			float v = ofMap(xy.y, -10, 10, 0, 1);
			float i = u * specular->getWidth() - 0.5;
			float j = v * specular->getHeight() - 0.5;
			float ipixel = fmod(i, specular->getWidth());
			float jpixel = fmod(j, specular->getHeight());
			return specular->getColor(ipixel, jpixel);
		}
		else {
			return specularColor;
		}
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

	ofPlanePrimitive plane;
	ofImage* texture;
	ofImage* specular;
	ofImage* bump;
	ofImage* displ;
	glm::vec3 normal;
	float width = 20;
	float height = 20;

};

// view plane for render camera
// 
class  ViewPlane : public Plane {
public:
	ViewPlane(glm::vec2 p0, glm::vec2 p1) { min = p0; max = p1; }

	ViewPlane() {                         // create reasonable defaults (6x4 aspect)
		/*min = glm::vec2(-3, -2);
		max = glm::vec2(3, 2);*/
		min = glm::vec2(-3, -.25);
		max = glm::vec2(3, 3.75);
		position = glm::vec3(0, 0, 5);
		normal = glm::vec3(0, 0, 1);      // viewplane currently limited to Z axis orientation
	}

	void setSize(glm::vec2 min, glm::vec2 max) { this->min = min; this->max = max; }
	float getAspect() { return width() / height(); }

	glm::vec3 toWorld(float u, float v);   //   (u, v) --> (x, y, z) [ world space ]

	void draw() {
		ofNoFill();
		ofDrawRectangle(glm::vec3(min.x, min.y, position.z), width(), height());
		ofFill();
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
	glm::vec2 bottomLeft() { return min; }
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
class RenderCam : public SceneObject {
public:
	RenderCam() {
		position = glm::vec3(0, 1, 10);
		aim = glm::vec3(0, 0, 0);
	}
	Ray getRay(float u, float v);
	void draw() {
		ofNoFill();
		ofDrawBox(position, 1.0);
		ofFill();
	};
	void drawFrustum();

	glm::vec3 aim;
	ViewPlane view;          // The camera viewplane, this is the view that we will render 
};



class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void ofApp::mouseScrolled(int x, int y, float ScrollX, float ScrollY);
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
	ofColor lambert(const glm::vec3& p, const glm::vec3& norm, const ofColor diffuse, float illumnation);
	ofColor phong(const glm::vec3& p, const glm::vec3& norm, const ofColor diffuse, const ofColor specular, float power, float illumination);
	bool ShadowGeneration(Ray& ray, glm::vec3 point, glm::vec3 normal, Light l);
	ofColor texture_lookup(ofTexture t, float u, float v);




	bool bHide = true;
	bool bShowImage = false;
	bool lightSelected = false;
	bool aimSelected = false;
	bool objSelected = false;

	int lightIndex;
	int objIndex;

	ofEasyCam  mainCam;
	ofCamera sideCam;
	ofCamera previewCam;
	ofCamera* theCam;    // set to current camera either mainCam or sideCams

	// set up one render camera to render image through
	//
	RenderCam renderCam;
	ofImage image;

	vector<Ray> rays;
	vector<SceneObject*> scene;
	vector<SceneObject*> objects;
	vector<Light*> lights;
	//vector<SpotLight*> spotlights;

	int imageWidth = 1200;
	int imageHeight = 800;


	ofxFloatSlider phongPower;
	ofxFloatSlider lightIntensity;
	ofxIntSlider red;
	ofxIntSlider green;
	ofxIntSlider blue;
	ofParameter<bool> bumpBool;
	ofParameter<bool> diffuseBool;
	ofParameter<bool> specularBool;


	ofxPanel gui;
	ofImage imageTexture;
	ofImage imageSpecular;
	ofImage imageBump;
	ofImage imageDisp;
	ofImage imageTexture2;
	ofImage imageSpecular2;
	ofImage imageBump2;
	ofImage imageTexture3;
	ofImage imageSpecular3;
	ofImage imageBump3;
};