//An Tran #014728649

#include "ofApp.h"
#include <iostream>
#include <glm/gtx/intersect.hpp>

//--------------------------------------------------------------
void ofApp::setup() {

	//gui setup
	gui.setup();

	//Adding GUI boxes
	gui.add(lightIntensity.setup("intensity", 10, 5, 40));
	gui.add(phongPower.setup("phong", 10.0, 10.0, 1000.0));
	gui.add(red.setup("red", 0, 0, 255));
	gui.add(green.setup("green", 0, 0, 255));
	gui.add(blue.setup("blue", 0, 0, 255));
	gui.add(bumpBool.set("bump", true));
	gui.add(diffuseBool.set("diffuse", true));
	gui.add(specularBool.set("specular", true));

	//Setting up background color
	ofSetBackgroundColor(ofColor::black);

	/**************************Adding objects to scene vector********************************/

	//Bump plus specular plus diffuse
	/*scene.push_back(new Plane(NULL, &imageBump, &imageSpecular, &imageTexture, glm::vec3(0, 8, -8), glm::vec3(0, 0, 1), ofColor::gray));
	scene.push_back(new Plane(NULL, &imageBump3, &imageSpecular3, &imageTexture3, glm::vec3(0, -.25, 0), glm::vec3(0, 1, 0), ofColor::gray));*/

	//bump plus specular
	/*scene.push_back(new Plane(NULL, &imageBump3, &imageSpecular3, NULL, glm::vec3(0, 8, -8), glm::vec3(0, 0, 1), ofColor::gray));
	scene.push_back(new Plane(NULL, &imageBump3, &imageSpecular3, NULL, glm::vec3(0, -.25, 0), glm::vec3(0, 1, 0), ofColor::gray));*/

	//Only bump map
	scene.push_back(new Plane(NULL, &imageBump3, NULL, NULL, glm::vec3(0, 8, -8), glm::vec3(0, 0, 1), ofColor::gray));
	scene.push_back(new Plane(NULL, &imageBump3, NULL, NULL, glm::vec3(0, -.25, 0), glm::vec3(0, 1, 0), ofColor::gray));

	//No texturing
	/*scene.push_back(new Plane(NULL, NULL, NULL, NULL, glm::vec3(0, 8, -8), glm::vec3(0, 0, 1), ofColor::gray));
	scene.push_back(new Plane(NULL, NULL, NULL, NULL, glm::vec3(0, -.25, 0), glm::vec3(0, 1, 0), ofColor::gray));*/

	Sphere* o1 = new Sphere(NULL, &imageSpecular2, &imageTexture2, glm::vec3(-0.5, 1, 0), .75, ofColor::blue);
	scene.push_back(o1);
	objects.push_back(o1);
	scene.push_back(&renderCam);

	//Adding lights to scene vector
	Light* l1 = new Light(glm::vec3(-3, 1, 5), .2, 25);
	Light* l2 = new Light(glm::vec3(0, 3, 0), .2, 25);
	Light* l3 = new Light(glm::vec3(6, 4, -3), .2, 25);
	//lights.push_back(l1);
	//scene.push_back(l1);
	lights.push_back(l2);
	scene.push_back(l2);
	//lights.push_back(l3);
	//scene.push_back(l3);

	//Adding spotlights to scene vector
	SpotLight* s1 = new SpotLight(glm::vec3(1, 7, 0), glm::vec3(0, 0, 0), .5, 25, .5);
	//spotlights.push_back(s1);
	//lights.push_back(s1);
	//scene.push_back(s1);

	//Setting up sideCam
	sideCam.setNearClip(0.1);
	sideCam.setFov(62);
	sideCam.setPosition(glm::vec3(10, 1, 0));

	//Setting up previewCam
	previewCam.setNearClip(0.1);
	previewCam.setFov(62);
	previewCam.setPosition(renderCam.position);

	//Setting up mainCam
	mainCam.setNearClip(0.1);
	mainCam.setFov(62);
	mainCam.setDistance(10);



	//load textures

	//texture 1
	imageTexture.load("44_marble floor_DIFF.jpg");
	imageSpecular.load("44_marble floor_SPEC.jpg");
	imageBump.load("44_marble floor_NORM.jpg");

	//texture 2
	imageTexture2.load("07_wood grain PBR texture_DIFF.jpg");
	imageSpecular2.load("07_wood grain PBR texture_SPEC.jpg");
	imageBump2.load("07_wood grain PBR texture_NORM.jpg");

	//texture 3
	imageTexture3.load("45_flower wall_DIFF.jpg");
	imageSpecular3.load("45_flower wall_SPEC.jpg");
	imageBump3.load("45_flower wall_NORM.jpg");
	imageDisp.load("45_flower wall_DISPL.jpg");
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	//disable depth test to draw GUI
	ofSetDepthTest(FALSE);

	gui.draw();

	//setting up depth test
	ofSetDepthTest(TRUE);

	//Starting cameras
	theCam = &mainCam;
	sideCam.begin();
	previewCam.begin();
	theCam->begin();

	//Draw each scene object into view
	ofSetColor(ofColor::gray);
	for (int i = 0; i < scene.size(); i++) {
		if (objSelected) {
			if (objects[objIndex] == scene[i]) {
				ofSetColor(ofColor::white);
			}
			else {
				ofSetColor(scene[i]->diffuseColor);
			}
		}
		else if (lightSelected) {
			if (lights[lightIndex] == scene[i]) {
				ofSetColor(ofColor::white);
			}
			else {
				ofSetColor(scene[i]->diffuseColor);
			}
		}
		else {
			ofSetColor(scene[i]->diffuseColor);
		}
		//ofSetColor(scene[i]->diffuseColor);
		scene[i]->draw();
	}

	//Draw axis
	drawAxis(glm::vec3(0, 0, 0));


	//ending cameras
	sideCam.end();
	previewCam.end();
	theCam->end();
}

//--------------------------------------------------------------
void ofApp::drawAxis(glm::vec3 position) {
	ofSetColor(ofColor::red);
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
	ofSetColor(ofColor::green);
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	ofSetColor(ofColor::blue);
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	ofSetColor(ofColor::white);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == 'd') {	// Delete selected object
		int sceneIndex;
		if (lightSelected && !aimSelected && !objSelected) {
			for (int i = 0; i < scene.size(); i++) {
				if (scene[i] == lights[lightIndex]) {
					sceneIndex = i;
				}
			}

			scene.erase(scene.begin() + sceneIndex);
			lights.erase(lights.begin() + lightIndex);
			lightSelected = false;
		}
		else if (objSelected && !lightSelected && !aimSelected) {
			for (int i = 0; i < scene.size(); i++) {
				if (scene[i] == objects[objIndex]) {
					sceneIndex = i;
				}
			}

			scene.erase(scene.begin() + sceneIndex);
			objects.erase(objects.begin() + objIndex);
			objSelected = false;
		}

	}

	if (key == 'j') {	// Add new sphere object
		int sceneIndex;
		if (!lightSelected && !aimSelected && !objSelected) {
			glm::vec3 screen3DPt = mainCam.screenToWorld(glm::vec3(ofGetMouseX(), ofGetMouseY(), 0));
			glm::vec3 rayOrigin = mainCam.getPosition();
			glm::vec3 rayDir = glm::normalize(screen3DPt - rayOrigin);
			glm::vec3 intersectNormal, intersectPoint;
			Ray r = Ray(rayOrigin, rayDir);

			Plane slPlane = Plane(NULL, NULL, NULL, NULL, glm::vec3(0, 0, 0), mainCam.getZAxis(), ofColor::gray);
			slPlane.intersect(r, intersectPoint, intersectNormal);

			Sphere* s = new Sphere(NULL, NULL, NULL, intersectPoint, 1, ofColor(red, green, blue));
			objects.push_back(s);
			scene.push_back(s);
		}
	}

	if (key == 'k') {	// Add new spotlight
		if (!lightSelected && !aimSelected && !objSelected) {
			glm::vec3 screen3DPt = mainCam.screenToWorld(glm::vec3(ofGetMouseX(), ofGetMouseY(), 0));
			glm::vec3 rayOrigin = mainCam.getPosition();
			glm::vec3 rayDir = glm::normalize(screen3DPt - rayOrigin);
			glm::vec3 intersectNormal, intersectPoint;
			Ray r = Ray(rayOrigin, rayDir);

			Plane slPlane = Plane(NULL, NULL, NULL, NULL, glm::vec3(0, 0, 0), mainCam.getZAxis(), ofColor::gray);
			slPlane.intersect(r, intersectPoint, intersectNormal);
			SpotLight* l = new SpotLight(intersectPoint, glm::vec3(intersectPoint.x + 1, intersectPoint.y - 3, intersectPoint.z), .5, 25, .5);
			lights.push_back(l);
			scene.push_back(l);
		}
	}

	if (key == 'l') {	// Add new point light
		if (!lightSelected && !aimSelected && !objSelected) {
			glm::vec3 screen3DPt = mainCam.screenToWorld(glm::vec3(ofGetMouseX(), ofGetMouseY(), 0));
			glm::vec3 rayOrigin = mainCam.getPosition();
			glm::vec3 rayDir = glm::normalize(screen3DPt - rayOrigin);
			glm::vec3 intersectNormal, intersectPoint;
			Ray r = Ray(rayOrigin, rayDir);

			Plane slPlane = Plane(NULL, NULL, NULL, NULL, glm::vec3(0, 0, 0), mainCam.getZAxis(), ofColor::gray);
			slPlane.intersect(r, intersectPoint, intersectNormal);
			Light* l = new Light(intersectPoint, .2, 25);
			lights.push_back(l);
			scene.push_back(l);
		}
	}


	//Switch to side view
	if (key == 's') {
		theCam->setPosition(sideCam.getPosition());
		theCam->lookAt(glm::vec3(0, 0, 0));
		mainCam.disableMouseInput();
	}

	//switch to mainCam
	if (key == 'm') {
		mainCam.reset();
		mainCam.enableMouseInput();
		mainCam.setNearClip(0.1);
		mainCam.setFov(90);
		mainCam.setDistance(10);
		mainCam.begin();
		mainCam.end();
	}

	//Start raytracing
	if (key == 'r') {
		rayTrace();
	}

	//Switch to previewCam
	if (key == 'p') {
		mainCam.setPosition(previewCam.getPosition());
		mainCam.lookAt(renderCam.aim);
		mainCam.disableMouseInput();
	}

	//pause mouse movement to adjust spotlights
	if (key == 'c') {
		if (mainCam.getMouseInputEnabled()) mainCam.disableMouseInput();
		else mainCam.enableMouseInput();
	}
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float ScrollX, float ScrollY) {
	/*
	* if a spot light is selected
	* then change the cone angle when the mouse is scrolled
	*/
	if (lightSelected) {
		if (ScrollY == 1) {
			lights[lightIndex]->increaseAngle();
		}
		if (ScrollY == -1) {
			lights[lightIndex]->decreaseAngle();
		}
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

	/*
	* if a spotlight is selected
	* then intersect a ray from the camera to a imaginary plane with the camera's z-axis
	* and move the spotlight along the imaginary plane
	*/
	if (lightSelected) {
		glm::vec3 screen3DPt = mainCam.screenToWorld(glm::vec3(x, y, 0));
		glm::vec3 rayOrigin = mainCam.getPosition();
		glm::vec3 rayDir = glm::normalize(screen3DPt - rayOrigin);
		glm::vec3 intersectNormal, intersectPoint;
		Ray r = Ray(rayOrigin, rayDir);


		Plane slPlane = Plane(NULL, NULL, NULL, NULL, glm::vec3(0, 0, 0), mainCam.getZAxis(), ofColor::gray);
		slPlane.intersect(r, intersectPoint, intersectNormal);


		lights[lightIndex]->setPosition(intersectPoint);
	}

	/*
	* if the aim point is selected
	* then interesct a ray with the a horizontal plane @ (0,0,0) with z-axis along (0,1,0)
	* and move the point along that imaginary plane
	*/
	if (aimSelected) {
		glm::vec3 screen3DPt = mainCam.screenToWorld(glm::vec3(x, y, 0));
		glm::vec3 rayOrigin = mainCam.getPosition();
		glm::vec3 rayDir = glm::normalize(screen3DPt - rayOrigin);
		glm::vec3 intersectNormal, intersectPoint;
		Ray r = Ray(rayOrigin, rayDir);

		Plane aimPlane = Plane(NULL, NULL, NULL, NULL, glm::vec3(0, 0, 0), mainCam.getZAxis(), ofColor::gray);

		aimPlane.intersect(r, intersectPoint, intersectNormal);
		lights[lightIndex]->setAim(intersectPoint);
	}

	if (objSelected) {
		glm::vec3 screen3DPt = mainCam.screenToWorld(glm::vec3(x, y, 0));
		glm::vec3 rayOrigin = mainCam.getPosition();
		glm::vec3 rayDir = glm::normalize(screen3DPt - rayOrigin);
		glm::vec3 intersectNormal, intersectPoint;
		Ray r = Ray(rayOrigin, rayDir);

		Plane aimPlane = Plane(NULL, NULL, NULL, NULL, glm::vec3(0, 0, 0), mainCam.getZAxis(), ofColor::gray);

		aimPlane.intersect(r, intersectPoint, intersectNormal);
		objects[objIndex]->setPosition(intersectPoint);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	glm::vec3 screen3DPt = mainCam.screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 rayOrigin = mainCam.getPosition();
	glm::vec3 rayDir = glm::normalize(screen3DPt - rayOrigin);
	glm::vec3 intersectNormal, intersectPoint;

	lightSelected = FALSE;												//set to false initially
	aimSelected = FALSE;													//set to false initially
	objSelected = FALSE;


	/*
	* for each spotlight
	* check if ray from screen intersects one of the spotlights or aim points
	* if yes, then save index of spotlight, and set respective boolean to true
	*/
	for (int i = 0; i < lights.size(); i++) {
		if (lights[i]->intersect(rayOrigin, rayDir, lights[i], intersectNormal, intersectPoint)) {
			lightSelected = TRUE;
			lightIndex = i;
			//cout << "hit" << endl;
		}

		if (lights[i]->intersectAim(rayOrigin, rayDir, lights[i], intersectNormal, intersectPoint)) {
			aimSelected = TRUE;
			lightIndex = i;
			//cout << "hit" << endl;
		}
	}

	for (int i = 0; i < objects.size(); i++) {
		Ray r = Ray(rayOrigin, rayDir);
		if (objects[i]->intersect(r, intersectNormal, intersectPoint)) {
			objIndex = i;
			objSelected = TRUE;
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

void ofApp::rayTrace() {
	//allocate space for image
	image.allocate(imageWidth, imageHeight, ofImageType::OF_IMAGE_COLOR);

	cout << "rendering image" << endl;

	//for each pixel in the image
	// check if ray intersects object
	//and determine closest object and change pixel color to closest
	//object color
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			Ray ray = renderCam.getRay((j + 0.5) / imageWidth, (i + 0.5) / imageHeight);
			glm::vec3 normal, point;											//normal of intersection point, point of intersection
			bool noHit = true;													//boolean if ray misses all objects
			float bestDistance = FLT_MAX;										//variable holds lowest distance for each object the ray hits

			//for each object in the scene
			//checks if ray intersects the object
			for (int k = 0; k < scene.size(); k++) {
				if (scene[k]->intersect(ray, point, normal)) {
					float tempDistance = glm::distance(point, renderCam.position);	//grabbing distance from current point to rendercam
					if (tempDistance < bestDistance) {								//compares temp distance to current lowest distance, if temp is lower
						bestDistance = tempDistance;								//set new lowest distance
						ofColor totalPhong = ofColor::black;
						ofColor totalLambert = ofColor::black;
						ofColor ambient = scene[k]->diffuseTextureLookup(point) * .2;
						ofColor bump = scene[k]->bumpTextureLookup(point);

						//Bump map prototype
						/*ofColor right = scene[k]->bumpright(point);
						ofColor down = scene[k]->bumpdown(point);*/



						float Bv = bump.r; //grab Bv from normal map
						float Bu = bump.g; //grab Bu from normal map

						// Check if bump mapping is selected
						if (bumpBool) {
							if (bump != NULL) {
								if (normal == glm::vec3(0, 0, 1)) {
									normal = normal + (Bv * glm::vec3(1, 0, 0)) + (Bu * glm::vec3(0, 0, 1));
									//normal = normal + ((bump.r - right.r)/255 * glm::vec3(1, 0, 0)) + ((bump.r - down.r)/255 * glm::vec3(0, 0, 1));
								}
								else if (normal == glm::vec3(0, 1, 0)) {
									normal = normal + (Bv * glm::vec3(0, 0, 1)) + (Bu * glm::vec3(0, 1, 0));
									//normal = normal + ((bump.r - right.r) * glm::vec3(0, 0, 1)) + ((bump.r - down.r) * glm::vec3(0, 1, 0));
								}
							}
						}

						Ray viewRay = Ray(point, glm::normalize(renderCam.position - point));		//ray from point of intersection to the render cam

						/*
						* loops through each light in scene to add up shading values
						*/
						for (int m = 0; m < lights.size(); m++) {

							float radius = glm::distance(point, lights[m]->position);			//gets distance from light to intersection point						
							float brightness = (lightIntensity / pow(radius, 2));			//gets brightness constant used in equations

							glm::vec3 l = glm::normalize(lights[m]->position - point);													//lightray direction from intersection point
							Ray lightray = Ray(point + (0.0001 * normal), glm::normalize(lights[m]->position - point));					//ray from point of intersection to the light
							glm::vec3 h = glm::normalize(l + viewRay.d);																//bisector ray from intersection point

							Ray lightray2 = Ray(lights[m]->position, glm::normalize(point - lights[m]->position));					//ray from spotlight position to interesction point
							glm::vec3 slnorm = glm::normalize(lights[m]->aimPos - lights[m]->position);								//direction vector from spotlight position to aim position

							float la = acos(glm::dot(lightray2.d, slnorm));

							/*
							* if light ray is not blocked
							* calculate lambert and phong shading
							*/
							if (la < lights[m]->lightangle()) {
								if (!ShadowGeneration(lightray, point, normal, *lights[m])) {
									totalLambert += lambert(l, normal, scene[k]->diffuseTextureLookup(point), brightness);																			//accumulate lambert shading
									totalPhong += phong(h, normal, scene[k]->diffuseTextureLookup(point), scene[k]->specularTextureLookup(point), phongPower, brightness);			//accumulate phong shading
								}
							}
						}

						if (diffuseBool && !specularBool) {	//diffuse only
							image.setColor(j, (imageHeight - 1) - i, ambient + totalLambert);
						}
						else if (specularBool && !diffuseBool) {	//specular only
							image.setColor(j, (imageHeight - 1) - i, ambient + totalPhong);
						}
						else if (diffuseBool && specularBool) {	//diffuse plus specular
							image.setColor(j, (imageHeight - 1) - i, ambient + totalLambert + totalPhong);
						}
						else { //ambient only
							image.setColor(j, (imageHeight - 1) - i, ambient);
						}
						//image.setColor(j, (imageHeight - 1) - i, ambient + totalLambert + totalPhong);	//set pixel(j,i) to color shaded with phong, lambert, and ambient
					}
					noHit = false;		//ray hit an object so sets noHit boolean to false
				}

			}
			if (noHit) {
				image.setColor(j, (imageHeight - 1) - i, ofColor::black);	//sets pixel color to black if ray hit nothing
			}
		}
	}

	cout << "saving image" << endl;

	image.save("image.png", OF_IMAGE_QUALITY_BEST);	//saving image

	cout << "done" << endl;
}

//Lambert shade pixel
//
ofColor ofApp::lambert(const glm::vec3& p, const glm::vec3& norm, const ofColor diffuse, float illumination) {

	glm::vec3 n = glm::normalize(norm);								//normalize normal vector for dot product
	//glm::vec3 n = norm;

	float dotproduct = (p.x * n.x) + (p.y * n.y) + (p.z * n.z);		//cos of angle between view ray and normal
	//cout << dotproduct << endl;
	return diffuse * (illumination) * max(0.0f, dotproduct);
}

//Phong shade pixel
//
ofColor ofApp::phong(const glm::vec3& p, const glm::vec3& norm, const ofColor diffuse, const ofColor specular, float power, float illumination) {

	glm::vec3 n = glm::normalize(norm);											//normalize normal vector for dot product
	//glm::vec3 n = norm;

	float dotproduct = (p.x * n.x) + (p.y * n.y) + (p.z * n.z);					//cos of angle between normal and bisector view ray and light ray

	return specular * (illumination) * pow(max(0.0f, dotproduct), power);
}

ofColor ofApp::texture_lookup(ofTexture t, float u, float v) {
	int i = u * t.getWidth() - 0.5;
	int j = v * t.getHeight() - 0.5;
	return ofColor::blanchedAlmond;
}

//Shadow boolean
//
bool ofApp::ShadowGeneration(Ray& ray, glm::vec3 point, glm::vec3 normal, Light l) {
	glm::vec3 pt;
	glm::vec3 norm;

	/*
	* for every object in the scene except planes,
	* check if the light ray interesects the object
	*/
	for (int i = 0; i < scene.size(); i++) {
		if (scene[i]->intersect(ray, pt, norm)) {
			float d = glm::distance(l.position, ray.p);	//distance from light position to point of intersection
			float c = glm::distance(pt, ray.p);			//distance from point of intersection to new point of intersection initialized by intersect function

			/*
			* if d < c
			* then it means object that was intersected was behind the light and should therefore be ignored
			* hence the do nothing
			*/
			if (d < c) {
				//do nothing
			}
			else {
				//cout << "true" << endl;
				return true;
			}
		}
	}
	return false;
}


// Intersect Ray with Plane  (wrapper on glm::intersect*)
//
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
		glm::vec2 yrange = glm::vec2(position.y - width / 2, position.y + width
			/ 2);
		glm::vec2 zrange = glm::vec2(position.z - height / 2, position.z +
			height / 2);
		// horizontal 
		//
		if (normal == glm::vec3(0, 1, 0) || normal == glm::vec3(0, -1, 0)) {
			if (point.x < xrange[1] && point.x > xrange[0] && point.z <
				zrange[1] && point.z > zrange[0]) {
				insidePlane = true;
			}
		}
		// front or back
		//
		else if (normal == glm::vec3(0, 0, 1) || normal == glm::vec3(0, 0, -1))
		{
			if (point.x < xrange[1] && point.x > xrange[0] && point.y <
				yrange[1] && point.y > yrange[0]) {
				insidePlane = true;
			}
		}
		// left or right
		//
		else if (normal == glm::vec3(1, 0, 0) || normal == glm::vec3(-1, 0, 0))
		{
			if (point.y < yrange[1] && point.y > yrange[0] && point.z <
				zrange[1] && point.z > zrange[0]) {
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

