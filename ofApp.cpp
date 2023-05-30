#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}
//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	vector<glm::vec3> base_location = { glm::vec3() };
	auto noise_location = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
	ofColor face_color = ofColor(239);
	for (auto& location : base_location) {

		for (int radius = 50; radius <= 300; radius += 10) {

			this->setRingToMesh(this->face, this->frame, location, radius, 10, face_color);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * - 3);

	this->face.draw();

	ofSetColor(39);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float height, ofColor face_color) {

	int index = face_target.getNumVertices();

	for (int deg = 0; deg < 360; deg += 1) {

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			auto noise_value_y = ofNoise(
				glm::vec4(vertex.y * 0.001 + ofGetFrameNum() * 0.0025));
			auto rotation_y = glm::rotate(glm::mat4(), ofMap(noise_value_y, 0, 1, -PI * 2, PI * 2), glm::vec3(0, 1, 0));

			vertex = glm::vec4(vertex, 0) * rotation_y + glm::vec4(location, 0);
		}

		auto face_index = face_target.getNumVertices();
		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		for (int i = 0; i < vertices.size(); i++) {

			face_target.addColor(face_color);
		}

		auto frame_index = frame_target.getNumVertices();
		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}