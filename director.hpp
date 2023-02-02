//
//  director.hpp
//  Lab04 -Apollo-11-Visuals
//
//  Created by Samuel on 2/1/23.
//

#ifndef director_hpp
#define director_hpp

#include <stdio.h>

/**
 DIRECTOR
 
 From the UML class diagram for W3. Delete if not needed.
 */
class director {
    const int frames;
    
public:
    director();
    director(int f);
    
    void updateInfo();
};

#endif /* director_hpp */
