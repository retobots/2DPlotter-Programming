#include "GCodeParser.h"

GCodeParser::GCodeParser(MotionController &mc):_mc(mc){}
void GCodeParser::parseLine(const String &line){
    if(line.length()==0) return;
    int start = 0;
    while(start<line.length()){
        int spaceIndex=line.indexOf(' ', start);
        String word;
        if(spaceIndex==-1){
            word=line.substring(start);
            start=line.length();
        } else{
            word=line.substring(start, spaceIndex);
            start=spaceIndex+1;
        }
        if(word.length()==0) continue;
        char cmd=word[0];
        String value = word.substring(1);
        if(cmd=='G'||cmd=='g'){
            int gcode = value.toInt();
            handleGcode(gcode, line.substring(start));
            break;
        }
    }
}
void GCodeParser::handleGcode(int code, const String &args){
    float x=0, y=0, i=0, j=0, r=0, feed=0;
    bool hasX=false, hasY=false, hasI=false, hasJ=false, hasR=false, hasF=false;
    int idx = 0;
    while(idx < args.length()){
        if(args.charAt(idx) == ' '){idx++; continue;}
        char letter = args.charAt(idx);
        idx++;
        String number = "";
        while(idx<args.length()&&(isDigit(args.charAt(idx))||args.charAt(idx)=='.'||args.charAt(idx)=='-'||args.charAt(idx)=='+')){
            number += args.charAt(idx);
            idx++;
        }
        float val=number.toFloat();
        switch(letter){
            case 'X': x=val; hasX=true; break; 
            case 'Y': y=val; hasY=true; break;
            case 'I': i=val; hasI=true; break;
            case 'J': j=val; hasJ=true; break;
            case 'R': r=val; hasR=true; break;
            case 'F': feed=val; hasF=true; break;
            // Co the bo sung truc z o dayyyy
        }
    }
    if(code==0||code==1){
        // Di chuyen tuyen tinh (G0/G1)
        _mc.linearMove(hasX?x:_mc.getCurrentX(), hasY?y:_mc.getCurrentY(), hasF?feed:_mc.getFeedRate());
    }else if(code==2||code==3){
        // Di chuyển cung tròn (G2/G3)
        bool cw=(code==2);
        _mc.circularMove(x, y, i, j, cw, hasF?feed:_mc.getFeedRate());
    }else if(code==28){
        _mc.goHome();
    }
}
