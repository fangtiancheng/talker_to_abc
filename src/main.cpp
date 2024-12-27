#include <iostream>
#include <vector>
#include <filesystem>

#include <happly.h>
#include <cnpy.h>

#include <Alembic/Abc/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreOgawa/All.h>
namespace fs = std::filesystem;

int main(int argc, char* argv[]){
    if(argc != 4){
        std::cerr << "usage: talker_to_abc $npy_path $flame_path $out_file\n";
        return 1;
    }
    // 1. load npy file
    fs::path npy_file(argv[1]);
    auto frames = cnpy::npy_load(npy_file);
    // frames shape = [1, n_frames, 5023*3]
    size_t n_frames;
    if(frames.shape.size() == 2){
        n_frames = frames.shape[0];
    } else if(frames.shape.size() == 3){
        n_frames = frames.shape[1];
        if(frames.shape[0] != 1){
            std::cerr << "npy shape error\n";
            return 1;
        }
    } else{
        std::cerr << "npy shape error\n";
        return 1;
    }
    if(frames.shape.back() != 5023*3){// check shape
        std::cerr << "npy is not flame\n";
        return 1;
    }
    // 2. load ply file
    fs::path ply_file(argv[2]);
    auto flame = happly::PLYData(ply_file);

    // get face index
    auto faceIndices = flame.getFaceIndices();
    if (faceIndices.size() != 9976 || faceIndices[0].size() != 3) {
        std::cerr << "PLY file face indices mismatch\n";
        return 1;
    }
    std::vector<int32_t> indices;
    for (const auto& face : faceIndices) {
        indices.insert(indices.end(), face.begin(), face.end());
    }

    std::vector<int32_t> counts(faceIndices.size(), 3); // 三角面数

    // 3. convert to abc
    fs::path out_file(argv[3]);
    using namespace Alembic::Abc;
    using namespace Alembic::AbcGeom;
    // 创建 Alembic 写入器
    Alembic::AbcCoreOgawa::WriteArchive writeArchive;
    OArchive archive(writeArchive, out_file.string());
    OPolyMesh meshObj(OObject(archive, kTop), "flameMesh");

    // 顶点存储空间
    std::vector<Imath::V3f> vertices(5023);

    for (size_t i = 0; i < n_frames; ++i) {
        std::cout << "Processing frame: " << i << std::endl;  // 调试信息
        float* frame_data = frames.data<float>() + i * 5023 * 3;

        for (size_t j = 0; j < 5023; ++j) {
            vertices[j] = Imath::V3f(
                    frame_data[3 * j],
                    frame_data[3 * j + 1],
                    frame_data[3 * j + 2]
            );
        }

        OPolyMeshSchema::Sample meshSample(
        V3fArraySample(&vertices.front(), vertices.size()),
        Int32ArraySample(&indices.front(), indices.size()),
                Int32ArraySample(&counts.front(), counts.size()));

        meshObj.getSchema().set(meshSample);
    }

    return 0;
}