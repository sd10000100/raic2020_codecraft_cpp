#include "../model/Model.hpp"

struct BaseAction
{
    virtual Action getAction() const = 0;

};
