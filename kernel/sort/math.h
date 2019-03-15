


template <typename T>
inline T min(T value1, T value2) 
{ 
    T value;
    if (value1<value2){
        value=value1;
    }else{
        value=value2;
    }
    return value;
}

template <typename T>
inline T max(T value1, T value2) 
{ 
    T value;
    if (value1>value2){
        value=value1;
    }else{
        value=value2;
    }
    return value;
}

template <typename T>
inline T limit(T minimum, T maximum, T value) 
{
    return max(minimum, min(maximum, value));
}