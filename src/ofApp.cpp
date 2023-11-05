#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    paintIsOn = false;
    brushMode = 'a';
    
    previousMousePos = vec2(0,0);
    gestureDirection = vec2(0,0);
    gestureMagnitude = 0;
    
    prevMouseAmt = 0.9; //smooth the drawing by averaging prev and current mouse pos
    currentMouseAmount = 1 - prevMouseAmt;
    
    canvasFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    //this clears the FBO at the start
    canvasFbo.begin();
    
    // loads image of Tim
    timImage.load("tim.png");
    
    ofClear(ofColor(150, 150, 150, 255));
    canvasFbo.end();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // dynamic brush
    vec2 mousePos = vec2(mouseX,mouseY);
    
    //subtract previous mouse position vector from currrent pos
    vec2 gesture = mousePos - previousMousePos;
    gestureMagnitude = length(gesture);
    gestureDirection = normalize(gesture);
    
    //storing our mouse x & y in the previous mouse pos variable
    previousMousePos.x = prevMouseAmt*previousMousePos.x + mouseX*currentMouseAmount;
    previousMousePos.y = prevMouseAmt*previousMousePos.y + mouseY*currentMouseAmount;
    
    // set brush modes to key press
    if(paintIsOn == true){
        canvasFbo.begin();
        
        if(brushMode == 's'){
            simpleBrush(mouseX,mouseY);
            
        } else if(brushMode == 'e'){
            eraserBrush(mouseX, mouseY);
            
        } else if(brushMode == 'd'){
            dynamicBrush(previousMousePos.x, previousMousePos.y, gestureMagnitude, gestureDirection);
            
        } else if(brushMode == 'g'){
            gradientBrush(previousMousePos.x, previousMousePos.y, gestureMagnitude, gestureDirection);
            
        }else if(brushMode == 't'){
            timmyBrush(mouseX, mouseY, gestureMagnitude, gestureDirection);
        }
        
        canvasFbo.end();
    }
    // check what brush mode is on
    cout << brushMode << endl;
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    canvasFbo.draw(0,0);
    ofPopMatrix();
    
    ofPushMatrix();
    ofSetColor(0, 0, 200);
    ofDrawBitmapString("try a  imple brush...", 150, 20);
    ofDrawBitmapString("or a  inamic brush...", 150, 40);
    ofDrawBitmapString("even better! a  radient brush...", 150, 60);
    ofDrawBitmapString("perhaps a bit of  immy? ", 150, 80);
    ofDrawBitmapString("not good? no problem...  rase it all", 150, 100);
    ofPopMatrix();
    
    ofPushMatrix();
    ofSetColor(200, 0, 0);
    ofDrawBitmapString("S", 198, 20);
    ofDrawBitmapString("D", 191, 40);
    ofDrawBitmapString("G", 269, 60);
    ofDrawBitmapString("T ", 288, 80);
    ofDrawBitmapString("E", 340, 100);
    ofDrawBitmapString("PRESS", 50, 50);
    ofPopMatrix();
    
    //    // displays the position of the mouse
    //    string pixelCoords = "("+to_string(mouseX)+", "+to_string(mouseY)+")";  // creates a string with pixel position
    //    ofSetColor(255,0,255);     // set text color
    //    ofDrawBitmapString(pixelCoords, mouseX, mouseY); // write text with pixel position following the mouse
    
    // reset colour to default
    ofSetColor(255);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 's'){
        brushMode = 's';
    } else if (key == 'e'){
        brushMode = 'e';
    } else if (key == 'a'){
        brushMode = 'a';
    } else if (key == 'd'){
        brushMode = 'd';
    } else if (key == 'g'){
        brushMode = 'g';
    } else if(key == 't'){
        brushMode = 't';
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    paintIsOn = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    paintIsOn = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
/// Draws an ellipse at x & y coordinates
/// parameter x - the x coordinate of the mouse position
/// parameter y - the y coordinate of the mouse position
void ofApp::simpleBrush(float x, float y){
    
    ofSetColor(150, 110, 70, ofRandom(100));
    ofDrawEllipse(x,y,30,30);
    
}

void ofApp::eraserBrush(float x, float y){
    ofSetColor(150, 150, 150, 255);
    ofDrawEllipse(x,y,150,150);
}

/// Draws at x & y coordinates with scale and rotation affected by speed and direction
/// parameter x - the x coordinate of the mouse position
/// parameter y - the y coordinate of the mouse position
/// parameter speed - the distance between the mouse position and previous position
/// parameter direction - direction of mouse movement

void ofApp::dynamicBrush(float x, float y, float speed, vec2 direction){
    
    //map speed paramater to scale, clamp it in this range
    float scale = ofMap(speed,1,50,0.1,3,true);
    
    //rotation angle
    float angle = atan2(gestureDirection.y, gestureDirection.x);
    
    // lenght of ech line
    float lineLength = 5;
    
    // set max number of lines. High number for a fuller brush
    int lineCount = 100;
    
    ofNoFill();
    ofSetLineWidth(0.5);
    
    //multiply unit vector by clamped value
    //ofClamp is used to constrain a value within a specified range
    vec2 limited = direction * ofClamp(speed,1,30);
    
    ofPushMatrix();
    //translate to mouse position
    ofTranslate(x, y);
    // scaling and rotating
    ofScale(scale);
    ofRotateRad(angle);
    
    for (int i = 0; i < lineCount; i++) {
        // to control the lines
        // when i is 0 goes to 100(lineCount)
        // when i is 0 goes to max 5 (lineLength)
        float lineScale = ofMap(i, 0, lineCount, 0, lineLength);
        
        ofSetColor(0, 0, ofRandom( 200, 255), 50); //random green shades with transparency
        ofDrawLine(0, 0, limited.x + lineScale , limited.y);
        
        ofSetColor(ofRandom(200, 255), 0, 0, 50); //random blue shades with transparency
        ofRotateRad(-angle);//use negative angle to get line draw oposit (star shape)
        ofDrawLine(0, 0, limited.x - lineScale, limited.y);
    }
    ofPopMatrix();
}

/// Draws at x & y coordinates with scale and rotation affected by speed and direction
/// parameter x - the x coordinate of the mouse position
/// parameter y - the y coordinate of the mouse position
/// parameter speed - the distance between the mouse position and previous position
/// parameter direction - direction of mouse movement
void ofApp::gradientBrush(float x, float y, float speed, vec2 direction) {
    
    // set the max scale to a random value
    float maxScale = ofRandom(0.1, 1);
    
    // map the speed from 0.1 to max 20
    // set new range of 1 to 3 (ellipses closer when slow speed)
    float maxSpeed = ofMap(speed, 0.1, 20, 1, 3);
    
    // measure final scale
    float scale = maxSpeed * maxScale;
    
    //multiply unit vector by clamped value
    //ofClamp is used to constrain a value within a specified range
    //small value so the ellipses are closer
    vec2 limited = direction * ofClamp(speed, 0.1, 10);
    
    // rotation angle
    float angle = atan2(direction.y, direction.x);
    
    ofPushMatrix();
    //translate to mouse position
    ofTranslate(x, y);
    // scaling and rotating
    ofScale(scale);
    ofRotateRad(angle);
    
    // colour mapping according to speed
    // low speed = darker blue
    // fast speed = light blue
    float blueColour = ofMap(maxSpeed, 0, 10, 100, 255);
    ofSetColor(0, 0, blueColour);
    
    // constrain size of ellipses
    // when slow = smaller and when fast = bigger
    float size = ofClamp(maxSpeed, 1, 8);
    
    ofDrawEllipse(limited.x, limited.y, size, size);
    
    ofPopMatrix();
}

/// Draws at x & y coordinates with scale and rotation affected by speed and direction
/// parameter x - the x coordinate of the mouse position
/// parameter y - the y coordinate of the mouse position
/// parameter speed - the distance between the mouse position and previous position
/// parameter direction - direction of mouse movement
void ofApp::timmyBrush(float x, float y, float speed, vec2 direction) {
    
    // map speed to rotation angle
    // when speed goes from 0 to 0.5
    // angle goes from 0 to 180
    float angle = ofMap(speed, 0, 0.5, 0, PI);
    
    // calculate angle based on direction
    float directionAngle = atan2(direction.y, direction.x);
    
    // combine the 2 angles (speed and direction) to get the final rotation angle
    float finalAngle = angle + directionAngle;
    
    // map speed to size
    // when speed goes from 0 to 0.5
    // size goes between 1 and 5
    float size = ofMap(speed, 0, 0.5, 1, 5);
    
    // set colour to random
    int r = ofRandom(100, 255);
    int g = ofRandom(100, 255);
    int b = ofRandom(100, 255);
    
    // set colour, translate and rotate
    ofSetColor(r, g, b);
    ofTranslate(x, y);
    ofRotateRad(finalAngle);
    
    timImage.draw(0, 0, size);
    
}



