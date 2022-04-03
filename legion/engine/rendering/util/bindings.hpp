#pragma once
#include <vector>
#include <string>
#include <application/application.hpp>

#define SV_START 0

/* in 0  */  #define SV_TEXCOORD0      SV_START   
/* in 1  */  #define SV_TEXCOORD1      SV_TEXCOORD0 + 1     
/* in 2  */  #define SV_TEXCOORD2      SV_TEXCOORD1 + 1     
/* in 3  */  #define SV_TEXCOORD3      SV_TEXCOORD2 + 1     

/* in 4  */  #define SV_POSITION       SV_TEXCOORD3 + 1
/* in 5  */  #define SV_COLOR          SV_POSITION + 1
/* in 6  */  #define SV_NORMAL         SV_COLOR + 1     
/* in 7  */  #define SV_TANGENT        SV_NORMAL + 1      
/* in 8  */  #define SV_ENTITYID       SV_TANGENT + 1
/* in 9  */  #define SV_FRAMEID         SV_ENTITYID+ 1
/* in 10  */  #define SV_MODELMATRIX    SV_FRAMEID + 1


/* uniform 10  */  #define SV_VIEW           SV_MODELMATRIX + 1
/* uniform 11 */  #define SV_PROJECT        SV_VIEW + 1
/* uniform 12 */  #define SV_CAMPOS         SV_PROJECT + 1
/* uniform 13 */  #define SV_VIEWDIR        SV_CAMPOS + 1
/* uniform 14 */  #define SV_VIEWPORT       SV_VIEWDIR + 1

/* uniform 10  */  #define SV_CAMERA         SV_VIEW

/* buffer  0  */  #define SV_LIGHTS         SV_START
/* uniform 15 */  #define SV_LIGHTCOUNT     SV_VIEWPORT + 1
/* uniform 16 */  #define SV_SKYBOX         SV_LIGHTCOUNT + 1

/* uniform 17 */  #define SV_SCENECOLOR     SV_SKYBOX + 1
/* uniform 18 */  #define SV_SCENEDEPTH     SV_SCENECOLOR + 1
/* uniform 19 */  #define SV_SCENENORMAL    SV_SCENEDEPTH + 1
/* uniform 20 */  #define SV_SCENEPOSITION  SV_SCENENORMAL + 1
/* uniform 21 */  #define SV_HDROVERDRAW    SV_SCENEPOSITION + 1

/* uniform 25 */  #define SV_ALBEDO         SV_HDROVERDRAW + 4   
/* uniform 26 */  #define SV_NORMALHEIGHT   SV_ALBEDO + 1   
/* uniform 27 */  #define SV_MRDAO          SV_NORMALHEIGHT + 1    
/* uniform 28 */  #define SV_EMISSIVE       SV_MRDAO + 1        
/* uniform 29 */  #define SV_HEIGHTSCALE    SV_EMISSIVE + 1

/* uniform 25 */  #define SV_MATERIAL       SV_ALBEDO


/* attachment 0 */ #define FRAGMENT_ATTACHMENT  GL_COLOR_ATTACHMENT0
/* attachment 1 */ #define NORMAL_ATTACHMENT    GL_COLOR_ATTACHMENT1
/* attachment 2 */ #define POSITION_ATTACHMENT  GL_COLOR_ATTACHMENT2
/* attachment 3 */ #define OVERDRAW_ATTACHMENT  GL_COLOR_ATTACHMENT3

namespace legion::rendering::detail
{
    inline const std::vector<std::string>& get_default_defines()
    {
        static std::vector<std::string> defines;
        if (defines.empty())
        {
            defines.push_back("SV_TEXCOORD0=" + std::to_string(SV_TEXCOORD0));
            defines.push_back("SV_TEXCOORD1=" + std::to_string(SV_TEXCOORD1));
            defines.push_back("SV_TEXCOORD2=" + std::to_string(SV_TEXCOORD2));
            defines.push_back("SV_TEXCOORD3=" + std::to_string(SV_TEXCOORD3));

            defines.push_back("SV_POSITION=" +   std::to_string(SV_POSITION));
            defines.push_back("SV_COLOR=" + std::to_string(SV_COLOR));
            defines.push_back("SV_NORMAL=" + std::to_string(SV_NORMAL));
            defines.push_back("SV_TANGENT=" + std::to_string(SV_TANGENT));
            defines.push_back("SV_ENTITYID=" + std::to_string(SV_ENTITYID));
            defines.push_back("SV_FRAMEID=" + std::to_string(SV_FRAMEID));
            defines.push_back("SV_MODELMATRIX=" + std::to_string(SV_MODELMATRIX));

            defines.push_back("SV_VIEW=" + std::to_string(SV_VIEW));
            defines.push_back("SV_PROJECT=" + std::to_string(SV_PROJECT));
            defines.push_back("SV_CAMPOS=" + std::to_string(SV_CAMPOS));
            defines.push_back("SV_VIEWDIR=" + std::to_string(SV_VIEWDIR));
            defines.push_back("SV_VIEWPORT=" + std::to_string(SV_VIEWPORT));

            defines.push_back("SV_CAMERA=" + std::to_string(SV_CAMERA));

            defines.push_back("SV_LIGHTS=" + std::to_string(SV_LIGHTS));
            defines.push_back("SV_LIGHTCOUNT=" + std::to_string(SV_LIGHTCOUNT));
            defines.push_back("SV_SKYBOX=" + std::to_string(SV_SKYBOX));

            defines.push_back("SV_SCENECOLOR=" + std::to_string(SV_SCENECOLOR));
            defines.push_back("SV_SCENEDEPTH=" + std::to_string(SV_SCENEDEPTH));
            defines.push_back("SV_SCENENORMAL=" + std::to_string(SV_SCENENORMAL));
            defines.push_back("SV_SCENEPOSITION=" + std::to_string(SV_SCENEPOSITION));
            defines.push_back("SV_HDROVERDRAW=" + std::to_string(SV_HDROVERDRAW));

            defines.push_back("SV_ALBEDO=" + std::to_string(SV_ALBEDO));
            defines.push_back("SV_NORMALHEIGHT=" + std::to_string(SV_NORMALHEIGHT));
            defines.push_back("SV_MRDAO=" + std::to_string(SV_MRDAO));
            defines.push_back("SV_EMISSIVE=" + std::to_string(SV_EMISSIVE));
            defines.push_back("SV_HEIGHTSCALE=" + std::to_string(SV_HEIGHTSCALE));

            defines.push_back("SV_MATERIAL=" + std::to_string(SV_MATERIAL));


            defines.push_back("SV_FRAGMENTOUT=" + std::to_string(0));
            defines.push_back("SV_NORMALOUT=" + std::to_string(1));
            defines.push_back("SV_POSITIONOUT=" + std::to_string(2));
            defines.push_back("SV_HDROVERDRAWOUT=" + std::to_string(3));
        }

        return defines;
    }
}
