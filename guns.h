#ifndef GUNS_H
#define GUNS_H

typedef struct gun{
    int damange;
    int ammo;
    enum {pistol, rifle, shotgun} type;
}gun;

#endif // GUNS_H