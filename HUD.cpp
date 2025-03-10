#include "HUD.h"
#include <iostream>
#include <string> // for string and to_string()
#include <math.h>
namespace game {
	HUD::HUD(GameObject* h,GameObject*number,GameObject* e,GameObject* exp_bar, GameObject* exp_t, GameObject* lv_t, GameObject* player)
		 {
         hp=0;
         energy=0;
         exp=0;
         level=0;
         money=0;
         score=0;
         buff=true;
         collect=true;
         hp_spirt = h;
         number_spirt = number;
         energy_spirt = e;
         exp_spirt = exp_bar;
         exp_text = exp_t;
         lv_text = lv_t;

         player_ = player;
         Init();

	}

    void HUD::Init() {
        hp_spirt->SetScale(glm::vec2(2, 1));
        hp_spirt->SetNumFrame(glm::vec2(2, 1));


        number_spirt->SetScale(glm::vec2(0.5, 0.5));
        number_spirt->SetNumFrame(glm::vec2(5,2));

        energy_spirt->SetScale(glm::vec2(2, 1));
        energy_spirt->SetNumFrame(glm::vec2(2, 1));

        exp_spirt->SetScale(glm::vec2(5, 1));
        exp_spirt->SetNumFrame(glm::vec2(2, 1));
        exp_text->SetScale(glm::vec2(0.4, 0.4));
        lv_text->SetScale(glm::vec2(0.4, 0.4));


        hp = player_->GetHP();
        max_hp = player_->Get_Max_Hp();

        energy = player_->Get_Energy();
        max_energy = player_->Get_Max_Energy();

        exp = player_->Get_Exper();
        max_exp = player_->Get_Max_Exp();
        level = player_->Get_Level();
        money = player_->Get_Coin();


    }

    void HUD:: Render(glm::mat4 view_matrix, double current_time) {

        glm::mat4 HP_matrix= glm::translate(view_matrix, glm::vec3(-4.25f, 3.25f, 0));

        glm::mat4 Energy_matrix = glm::translate(view_matrix, glm::vec3(-4.25f, 3.2f, 0));

        glm::mat4 NUM_matrix = glm::translate(view_matrix, glm::vec3(-4.75f, 2.5f, 0));

        glm::mat4 EXP_matrix = glm::translate(view_matrix, glm::vec3(0, -3.75f, 0));
       
        glm::mat4 EXP_T_M = glm::translate(view_matrix, glm::vec3(-2.25, -3.5f, 0));
        glm::mat4 LV_T_M = glm::translate(view_matrix, glm::vec3(1.75, -3.45f, 0));
        
        float p = (float)GetHP() / (float)GetMax_HP();
        RenderBar(HP_matrix, current_time, hp_spirt, p);
        p = (float)GetEnergy() / (float)GetMax_Energy();
        //std::cout << p << std::endl;
        p = (17.0f * p + 9.0f) / 32.0f;
       // std::cout << p << std::endl;
        RenderBar(Energy_matrix, current_time, energy_spirt, p);
        p = (float)GetExp() / (float)GetMax_Exp();
        RenderBar(EXP_matrix, current_time, exp_spirt, p);
        RenderNumber(NUM_matrix, current_time,score);
        
        exp_text->Render(EXP_T_M, current_time);
        lv_text->Render(LV_T_M, current_time);

        LV_T_M= glm::translate(LV_T_M, glm::vec3(lv_text->GetScale().x, 0, 0));
        RenderNumber(LV_T_M, current_time, GetLevel());
        
    }

    void HUD::SetBar(GameObject* obj, float percent) {
        obj->SetBar_Percent(percent);
    }
    
    void HUD::RenderBar(glm::mat4 view_matrix, double current_time, GameObject* obj,float percent) {
        obj->SetBar_Percent(percent);
        obj->SetCurrentFrame(0);
        obj->Render(view_matrix, current_time);

        obj->SetBar_Percent(1.0);
        obj->SetCurrentFrame(1);
        obj->Render(view_matrix, current_time);

    }
    void HUD::RenderNumber(glm::mat4 view_matrix, double current_time,int number) {
        glm::vec2 scale = number_spirt->GetScale();
        int index =0;
        int temp = number;
        int result;
        int n;
        while (temp != 0) {
            temp = floor(temp / 10);
            index++;
        }
        for (int i = 0;i < 3;i++) {
            if ((3-i)>index) {
                number_spirt->SetCurrentFrame(9);
            }else {
                result = floor(number / pow((double)10, (double)(index-1)));
                int n = chooseFrame(result);
                number_spirt->SetCurrentFrame(n);
                number -= result * pow((double)10, (double)(index-1));
                index--;
            }
            number_spirt->Render(view_matrix, current_time);
            view_matrix = glm::translate(view_matrix, glm::vec3(scale.x, 0, 0));

        }

    }
    void HUD::RenderText(glm::mat4 view_matrix, double current_time) {

        

    }
    int HUD::chooseFrame(int n) {
        switch (n)
        {
        case 1:
            return 0;

        case 2:
            return 1;

        case 3:
            return 2;

        case 4:
            return 3;

        case 5:
            return 4;

        case 6:
            return 5;

        case 7:
            return 6;

        case 8:
            return 7;

        case 9:
            return 8;

        default:
            return 9;

        }
        return 0;
    }
	void  HUD::Update(double delta_time) {
        hp = player_->GetHP();
        max_hp = player_->Get_Max_Hp();

        energy = player_->Get_Energy();
        max_energy = player_->Get_Max_Energy();

        exp = player_->Get_Exper();
        max_exp = player_->Get_Max_Exp();
        level = player_->Get_Level();
        money = player_->Get_Coin();


	}
} // namespace game
