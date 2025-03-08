#ifndef HUD_H_
#define HUD_H_
#include "game_object.h"



namespace game {

    // Inherits from GameObject
    class HUD  {

    public:
        HUD(GameObject* hp,GameObject* num,GameObject* energy,GameObject* exp_bar,GameObject* exp_text,GameObject* lv_text);
        
        void Init();

        void SetHP(int h) { hp = h; }
        void SetEnergy(int e) { energy = e; }
        void SetExp(int e) { exp = e; }
        void SetLevel(int l) { level = l; }
        void SetMoney(int m) { money = m; }
        void SetBuff(bool b) { buff = b; }
        void SetCollect(bool c) { collect = c; }
        void SetScore(int s) { score = s; }
        void SetMax_HP(int h) { max_hp = h; }
        void SetMax_Energy(int e) { max_energy = e; }
        void SetMax_Exp(int e) { max_exp = e; }
        int GetHP() { return hp; }
        int GetEnergy() { return energy; }
        int GetExp() { return exp; }
        int GetLevel() { return level; }
        int GetMoney() { return money; }
        bool GetBuff() { return buff; }
        bool GetCollect() { return collect; }
        int GetScore() { return score; }

        int GetMax_HP() { return max_hp; }
        int GetMax_Energy() { return max_energy; }
        int GetMax_Exp() { return max_exp; }

        void Update(double delta_time) ;
        void Render(glm::mat4 view_matrix, double current_time);
        void RenderText(glm::mat4 view_matrix,double current_time);

        void RenderNumber(glm::mat4 view_matrix, double current_time, int number);
        void RenderBar(glm::mat4 view_matrix, double current_time, GameObject* obj,float percent);
        int chooseFrame(int n);

        void SetBar(GameObject* obj, float percent);

    private:
        int hp;
        int energy;
        int exp;
        int level;
        int money;
        int score;
        bool buff;
        bool collect;
        int max_hp;
        int max_energy;
        int max_exp;

        GameObject* hp_spirt;
        GameObject* energy_spirt;
        GameObject* number_spirt;
        GameObject* exp_spirt;
        GameObject* exp_text;
        GameObject* lv_text;

        


    }; // class HUD

} // namespace game

#endif // HUD_H_
