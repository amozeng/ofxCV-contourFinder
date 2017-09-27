#include "ofApp.h"


void ofApp::setup()
{
    // The `hands.mp4` video used in these examples can be downloaded
    // [here](http://christopherbaker.net/dropbox/hands.mp4).
    cam.setup(640, 480);


    binaryPixels.allocate(cam.getWidth(),
                          cam.getHeight(),
                          OF_PIXELS_GRAY);

    binaryTexture.loadData(binaryPixels);

    gui.setup();
    gui.add(blurLevel.set("Blur Level", 17, 0, 30));
    gui.add(threshold.set("Threshold", 200, 0, 255));
    gui.add(erodeIterations.set("Erode Iterations", 0, 0, 30));
    gui.add(dilateIterations.set("Dilate Iterations", 0, 0, 30));
    gui.add(invert.set("Invert", true));

    gui.add(contourMinArea.set("Contour Min Area", 10, 1, 100));
    gui.add(contourMaxArea.set("Contour Max Area", 200, 1, 500));
    gui.add(contourThreshold.set("Contour Threshold", 128, 0, 255));
    gui.add(contourHoles.set("Contour Holes", true));
    gui.add(contourSimplify.set("Contour Simplify", true));

}


void ofApp::update()
{
    
    //------- ofxCV example cam
    cam.update();
    if(cam.isFrameNew()) {
        
        //------- chris example
        ofxCv::convertColor(cam, binaryPixels, CV_RGB2GRAY);
        ofxCv::blur(binaryPixels, binaryPixels, blurLevel);
        ofxCv::threshold(binaryPixels, binaryPixels, threshold, invert);
        ofxCv::erode(binaryPixels, binaryPixels, erodeIterations);
        ofxCv::dilate(binaryPixels, binaryPixels, dilateIterations);
        
        contourFinder.setMinAreaRadius(contourMinArea);
        contourFinder.setMaxAreaRadius(contourMaxArea);
        contourFinder.setThreshold(contourThreshold);
        contourFinder.setFindHoles(contourHoles);
        contourFinder.setSimplify(contourSimplify);
        contourFinder.findContours(binaryPixels);

        binaryTexture.loadData(binaryPixels);

    }
    
//    video.update();
//
//    if (video.isFrameNew())
//        
//        {
//            
//        ofxCv::convertColor(video, binaryPixels, CV_RGB2GRAY);
//        ofxCv::blur(binaryPixels, binaryPixels, blurLevel);
//        ofxCv::threshold(binaryPixels, binaryPixels, threshold, invert);
//        ofxCv::erode(binaryPixels, binaryPixels, erodeIterations);
//        ofxCv::dilate(binaryPixels, binaryPixels, dilateIterations);
//
//
//        contourFinder.setMinAreaRadius(contourMinArea);
//        contourFinder.setMaxAreaRadius(contourMaxArea);
//        contourFinder.setThreshold(contourThreshold);
//        contourFinder.setFindHoles(contourHoles);
//        contourFinder.setSimplify(contourSimplify);
//        contourFinder.findContours(binaryPixels);
//
//        binaryTexture.loadData(binaryPixels);
//    }
}


void ofApp::draw()

{
    cam.draw(0, 0);

    ofPushStyle();
        ofSetLineWidth(3);
        auto deadLabels = contourFinder.getTracker().getDeadLabels();

        holes.clear();
        
        for (auto i = 0; i < contourFinder.size(); ++i)
            
        {
            const ofPolyline contour = getPreparedContour(i);
            auto label = contourFinder.getLabel(i);
            auto age = contourFinder.getTracker().getAge(label);

            if (contourFinder.getHole(i))
            {
                ofSetColor(ofColor::blue);
                holes[label] = contourFinder.getPolyline(i);
                
            }
            else
            {
                ofSetColor(ofColor::green);
            }

            contourFinder.getPolyline(i).draw();

            ofDrawBitmapStringHighlight(ofToString(label) + " : " + ofToString(age), ofxCv::toOf(contourFinder.getCenter(i)));

        }
    ofPopStyle();

    binaryTexture.draw(640, 0);

    gui.draw();

    ofFill();
    
    
}
ofPolyline ofApp::getPreparedContour(std::size_t i) const
{
    // Get a copy of the contour.
    ofPolyline _contour = contourFinder.getPolylines()[i];
    
    // Simplify the contour.
    
    // Resample the contour by a reasonable amount.
    
    return _contour;
}
