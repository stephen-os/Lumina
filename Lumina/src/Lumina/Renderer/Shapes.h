//
//struct Square2D : Model {
//    Buffer vert, tex;                                         //2 Buffers
//    Square2D() :Model({ "in_Quad", "in_Tex" }),                  //2 Binding Points
//        vert({ -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f }),   //Initialize Buffer "vert"
//        tex({ 0.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f }) {    //Initialize Buffer "tex"
//        bind<glm::vec2>("in_Quad", &vert);                      //Bind vert as glm::vec2
//        bind<glm::vec2>("in_Tex", &tex);                        //Bind tex as glm::vec2
//        SIZE = 4;                                               //Set total vertex number to 4
//    }
//};
//
//struct Rectangle3D : Model {
//    Buffer vert, tex;
//    Square3D() :Model({ "in_Quad", "in_Tex" }),
//        vert({ -1.0f,  1.0f,  0.0f, -1.0f, -1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f, -1.0f,  0.0f }),
//        tex({ 0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  1.0f }) {
//        bind<glm::vec3>("in_Quad", &vert);
//        bind<glm::vec2>("in_Tex", &tex);
//        SIZE = 4;
//    }
//};
//
//struct Cube : Model {
//    Buffer vert, tex;
//    Cube() :Model({ "in_Quad", "in_Tex" }),
//        vert({ /* Front */ 1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,
//            /* Back  */-1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
//            /* Left  */-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
//            /* Right */ 1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,
//            /* Top   */-1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
//            /* Bottom*/ 1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f }),
//        tex({ 0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  1.0f }) {
//        bind<glm::vec3>("in_Quad", &vert);
//        bind<glm::vec2>("in_Tex", &tex);
//        SIZE = 36;
//    }
//};
//
//struct Gizmo : Model {
//    Buffer vert, tex;
//    Gizmo() :Model({ "in_Quad", "in_Tex" }),
//        vert({ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//              0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }),
//        tex({ 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//              0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//              0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }) {
//        bind<glm::vec3>("in_Quad", &vert);
//        bind<glm::vec3>("in_Tex", &tex);
//        SIZE = 6;
//    }
//};
//
//struct Triangle : Model {
//    Buffer vert;
//    Triangle() :Model({ "vert" }),
//        vert({ 1.0f, 0.0f, 0.0f, 0.0f,
//                    0.0f, 1.0f, 0.0f, 0.0f,
//                    0.0f, 0.0f, 1.0f, 0.0f,
//                    1.0f, 0.0f, 0.0f, 0.0f }) {
//        bind<glm::vec4>("vert", &vert);
//        SIZE = 4;
//    }
//}; #pragma once