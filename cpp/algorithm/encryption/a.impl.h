#define TEMPLATE_HEADER template<typename TypeA, typename TypeB>
#define TEMPLATE_PARAM TypeA, TypeB

TEMPLATE_HEADER
TypeB A<TEMPLATE_PARAM>::sum(TypeA a, TypeB b)
{
    return (static_cast<TypeB>(a) + b);
}
