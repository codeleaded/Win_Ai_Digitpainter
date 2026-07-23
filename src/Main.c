#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/GSprite.h"
#include "/home/codeleaded/System/Static/Library/NeuralNetwork.h"

#define SPRITE_PATH         "/home/codeleaded/Data/NN/Digits/"
#define DATA_PATH           "/home/codeleaded/Data/NN/DigitsGray/"
#define SPRITE_TEST         "testing"
#define SPRITE_TRAINING     "training"
#define SPRITE_COUNT        4
#define SPRITE_MAX          300

#define NN_PATH             "./data/Model.nnalx"
#define NN_COUNT            10
#define NN_LEARNRATE        0.1f

int prediction = 0;
GSprite sp;
NeuralNetwork nnet;

void Setup(AlxWindow* w){
    Random_Set(Time_Nano());

    ReloadAlxFont(16,16);
    sp = GSprite_None(28,28);
    
    nnet = NeuralNetwork_Make((NeuralLayerBuilder[]){
        NeuralLayerBuilder_Make(784,"relu"),
        NeuralLayerBuilder_Make(16,"relu"),
        NeuralLayerBuilder_Make(NN_COUNT,"softmax"),
        NeuralLayerBuilder_End()
    });
}
void Update(AlxWindow* w){
    if(Stroke(ALX_KEY_E).PRESSED){
        if(Files_isFile(NN_PATH)){
            NeuralNetwork_Free(&nnet);
            nnet = NeuralNetwork_Load(NN_PATH);
            printf("[NeuralNetwork]: Load -> Success!\n");
        }else{
            printf("[NeuralNetwork]: Load -> Failed!\n");
        }
    }

    const float posx = 0.5f * (GetWidth() - sp.w);
    const float posy = 0.5f * (GetHeight() - sp.h);
    const Vec2 m = Vec2_Sub(GetMouse(),(Vec2){ posx,posy });


    Clear(DARK_BLUE);

    GSprite_Render(WINDOW_STD_ARGS,&sp,posx,posy);

    if(Stroke(ALX_MOUSE_L).DOWN){
        if(m.x >= 0.0f && m.x < sp.w && m.y >= 0.0f && m.y < sp.h){
            sp.img[(int)m.y * sp.h + (int)m.x] = 1.0f;
        }

        NeuralNetwork_Calc(&nnet,sp.img);
        prediction = NeuralNetwork_Decision(&nnet);
    }
    if(Stroke(ALX_MOUSE_R).DOWN){
        if(m.x >= 0.0f && m.x < sp.w && m.y >= 0.0f && m.y < sp.h){
            sp.img[(int)m.y * sp.h + (int)m.x] = 0.0f;
        }

        NeuralNetwork_Calc(&nnet,sp.img);
        prediction = NeuralNetwork_Decision(&nnet);
    }

    //String str = String_Format("T:%d, ND:%d, I:%d",test,ndir,item);
    //RenderCStrSize(str.Memory,str.size,0.0f,0.0f,WHITE);
    //String_Free(&str);

    String str = String_Format("P: %d",prediction);
    RenderCStrSize(str.Memory,str.size,(GetWidth() - str.size * GetAlxFont()->CharSizeX) / 2,0.0f,WHITE);
    String_Free(&str);
}
void Delete(AlxWindow* w){
    NeuralNetwork_Free(&nnet);
    GSprite_Free(&sp);
}

int main(){
    if(Create("Ai Digitpainter",80,60,16,16,Setup,Update,Delete))
        Start();
    return 0;
}