#ifndef HASHEABLE_H
#define HASHEABLE_H


class Hasheable
{
    public:
        Hasheable() {}
        virtual ~Hasheable() {}
        virtual int getHashCode(){ return (int)this; }
    protected:
    private:
};

#endif // HASHEABLE_H
