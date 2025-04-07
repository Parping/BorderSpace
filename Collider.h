#ifndef COLLIDER_H_
#define COLLIDER_H_

#include "game_object.h"

namespace game {
   // class GameObject;
    class Collider {
    public:
        Collider() {};
        Collider(GameObject* g) { object = g; };
        void SetObject(GameObject* g) { object = g; };
        GameObject* GetObject() { return object; };
        virtual void SetRadius(float r) {};
        virtual void SetWidth(float w) {  };
        virtual void SetHeight(float h) { };
        virtual float GetRadius() { return 0; }
        virtual float GetWidth() { return 0; }
        virtual float GetHeight() { return 0; }
        virtual bool TestCollision(Collider& other) { return false; };
        //	virtual bool TestCollision(RactCollider& other) { return false; };
        virtual void CollisionEvent(GameObject* object1, GameObject* object2) {};

  //  protected:
       GameObject* object;
    };
    class CircleCollider : public Collider {
    private:
        float radius_;

    public:

        CircleCollider() {}
        CircleCollider(float r, GameObject* g);
        bool TestCollision(Collider& other) override;
        void SetRadius(float r) override { radius_ = r; };
        float GetRadius() override { return radius_; };

        void CollisionEvent(GameObject* object1, GameObject* object2)override;
    };

    class RactCollider : public
        Collider {
    private:
        float width;
        float height;

    public:

        float GetWidth() { return width; }
        float GetHeight() { return height; }

        void SetWidth(float w) override{ width = w; };
        void SetHeight(float h) override { height = h; };
        RactCollider() {}
        RactCollider(GameObject* g);
        RactCollider(float w, float h, GameObject* g);
        bool TestCollision(Collider& other)override;
    };
} // namespace game

#endif // COLLIDER_H_