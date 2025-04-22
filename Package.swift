// swift-tools-version: 5.10
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "DoorstepDropoffSDKPackage",
    products: [
        .library(
            name: "DoorstepDropoffSDKPackage",
            targets: ["DoorstepDropoffSDKPackage", "DoorstepAI"]),
    ],
    targets: [
        .target(
            name: "DoorstepDropoffSDKPackage"),
        .binaryTarget(name: "DoorstepAI", path: "./DoorstepDropoffSDK.xcframework")
    ]
)
