// THIS IS REQUIRED DUE TO THE DISPLAY I AM USING BEING PART OF A DAISY CHAIN WITH A ALPHANUMERIC 14 DIGIT DISPLAY AND USING A MAX 7219 THAT CANT HANDLE THAT MANY SEGMENTS. 
// IT USES THE DP AS A PART OF D

void needyDigitDisplay(byte maxnbr, byte d1, byte d2, byte needySec) 
{
    if (needySec==45) {
      lc.setDigit(maxnbr,d2,5,true); 
      lc.setDigit(maxnbr,d1,4,true); 
    } else if (needySec==44) {
      lc.setDigit(maxnbr,d2,4,true); 
      lc.setDigit(maxnbr,d1,4,true); 
    } else if (needySec==43) {
      lc.setDigit(maxnbr,d2,3,true); 
      lc.setDigit(maxnbr,d1,4,true); 
    } else if (needySec==42) {
      lc.setDigit(maxnbr,d2,2,true); 
      lc.setDigit(maxnbr,d1,4,true); 
    } else if (needySec==41) {
      lc.setDigit(maxnbr,d2,1,false); 
      lc.setDigit(maxnbr,d1,4,true); 
    } else if (needySec==40) {
      lc.setDigit(maxnbr,d2,0,false); 
      lc.setDigit(maxnbr,d1,4,true); 
    } else if (needySec==39) {
      lc.setDigit(maxnbr,d2,9,true); 
      lc.setDigit(maxnbr,d1,3,true); 
    } else if (needySec==38) {
      lc.setDigit(maxnbr,d2,8,true); 
      lc.setDigit(maxnbr,d1,3,true); 
    } else if (needySec==37) {
      lc.setDigit(maxnbr,d2,7,false); 
      lc.setDigit(maxnbr,d1,3,true); 
    } else if (needySec==36) {
      lc.setDigit(maxnbr,d2,6,true); 
      lc.setDigit(maxnbr,d1,3,true); 
    } else if (needySec==35) {
      lc.setDigit(maxnbr,d2,5,true); 
      lc.setDigit(maxnbr,d1,3,true); 
    } else if (needySec==34) {
      lc.setDigit(maxnbr,d2,4,true); 
      lc.setDigit(maxnbr,d1,3,true); 
    } else if (needySec==33) {
      lc.setDigit(maxnbr,d2,3,true); 
      lc.setDigit(maxnbr,d1,3,true); 
    } else if (needySec==32) {
      lc.setDigit(maxnbr,d2,2,true); 
      lc.setDigit(maxnbr,d1,3,true); 
    } else if (needySec==31) {
      lc.setDigit(maxnbr,d2,1,false); 
      lc.setDigit(maxnbr,d1,3,true); 
    } else if (needySec==30) {
      lc.setDigit(maxnbr,d2,0,false); 
      lc.setDigit(maxnbr,d1,3,true); 
    } else if (needySec==29) {
      lc.setDigit(maxnbr,d2,9,true); 
      lc.setDigit(maxnbr,d1,2,true); 
    } else if (needySec==28) {
      lc.setDigit(maxnbr,d2,8,true); 
      lc.setDigit(maxnbr,d1,2,true); 
    } else if (needySec==27) {
      lc.setDigit(maxnbr,d2,7,false); 
      lc.setDigit(maxnbr,d1,2,true); 
    } else if (needySec==26) {
      lc.setDigit(maxnbr,d2,6,true); 
      lc.setDigit(maxnbr,d1,2,true); 
    } else if (needySec==25) {
      lc.setDigit(maxnbr,d2,5,true); 
      lc.setDigit(maxnbr,d1,2,true); 
    } else if (needySec==24) {
      lc.setDigit(maxnbr,d2,4,true); 
      lc.setDigit(maxnbr,d1,2,true); 
    } else if (needySec==23) {
      lc.setDigit(maxnbr,d2,3,true); 
      lc.setDigit(maxnbr,d1,2,true); 
    } else if (needySec==22) {
      lc.setDigit(maxnbr,d2,2,true); 
      lc.setDigit(maxnbr,d1,2,true); 
    } else if (needySec==21) {
      lc.setDigit(maxnbr,d2,1,false); 
      lc.setDigit(maxnbr,d1,2,true); 
    } else if (needySec==20) {
      lc.setDigit(maxnbr,d2,0,false); 
      lc.setDigit(maxnbr,d1,2,true); 
    } else if (needySec==19) {
      lc.setDigit(maxnbr,d2,9,true); 
      lc.setDigit(maxnbr,d1,1,false); 
    } else if (needySec==18) {
      lc.setDigit(maxnbr,d2,8,true); 
      lc.setDigit(maxnbr,d1,1,false); 
    } else if (needySec==17) {
      lc.setDigit(maxnbr,d2,7,false); 
      lc.setDigit(maxnbr,d1,1,false); 
    } else if (needySec==16) {
      lc.setDigit(maxnbr,d2,6,true); 
      lc.setDigit(maxnbr,d1,1,false); 
    } else if (needySec==15) {
      lc.setDigit(maxnbr,d2,5,true); 
      lc.setDigit(maxnbr,d1,1,false); 
    } else if (needySec==14) {
      lc.setDigit(maxnbr,d2,4,true); 
      lc.setDigit(maxnbr,d1,1,false); 
    } else if (needySec==13) {
      lc.setDigit(maxnbr,d2,3,true); 
      lc.setDigit(maxnbr,d1,1,false); 
    } else if (needySec==12) {
      lc.setDigit(maxnbr,d2,2,true); 
      lc.setDigit(maxnbr,d1,1,false); 
    } else if (needySec==11) {
      lc.setDigit(maxnbr,d2,1,false); 
      lc.setDigit(maxnbr,d1,1,false); 
    } else if (needySec==10) {
      lc.setDigit(maxnbr,d2,0,false); 
      lc.setDigit(maxnbr,d1,1,false); 
    } else if (needySec==9) {
      lc.setDigit(maxnbr,d2,9,true); 
      lc.setDigit(maxnbr,d1,0,false); 
    } else if (needySec==8) {
      lc.setDigit(maxnbr,d2,8,true); 
      lc.setDigit(maxnbr,d1,0,false); 
    } else if (needySec==7) {
      lc.setDigit(maxnbr,d2,7,false); 
      lc.setDigit(maxnbr,d1,0,false); 
    } else if (needySec==6) {
      lc.setDigit(maxnbr,d2,6,true); 
      lc.setDigit(maxnbr,d1,0,false); 
    } else if (needySec==5) {
      lc.setDigit(maxnbr,d2,5,true); 
      lc.setDigit(maxnbr,d1,0,false); 
    } else if (needySec==4) {
      lc.setDigit(maxnbr,d2,4,true); 
      lc.setDigit(maxnbr,d1,0,false); 
    } else if (needySec==3) {
      lc.setDigit(maxnbr,d2,3,true); 
      lc.setDigit(maxnbr,d1,0,false); 
    } else if (needySec==2) {
      lc.setDigit(maxnbr,d2,2,true); 
      lc.setDigit(maxnbr,d1,0,false); 
    } else if (needySec==1) {
      lc.setDigit(maxnbr,d2,1,false); 
      lc.setDigit(maxnbr,d1,0,false); 
    } else if (needySec==0) {
      lc.setDigit(maxnbr,d2,0,false);
      lc.setDigit(maxnbr,d1,0,false);
    }
}
