package com.example.testndk.demo

class NativeClass {
    companion object {
        external fun faceDetection(
            matAddrRgba: Long,
            height: Int,
            rotation: Int,
            modelPath: String
        ): Boolean;

        external fun loadTensorflowModel()

        external fun testTensorflowLite(matAddrRgba: Long,)
    }
}