{
    "Source" : "Sticker.azsl",

    "DepthStencilState" : { 
        "Depth" : { "Enable" : false, "CompareFunc" : "GreaterEqual" }
    },

    "GlobalTargetBlendState" : {
      "Enable" : true,
      "BlendSource" : "One",
      "BlendAlphaSource" : "One",
      "BlendDest" : "AlphaSourceInverse",
      "BlendAlphaDest" : "AlphaSourceInverse",
      "BlendAlphaOp" : "Add"
    },

    "DrawList" : "sticker",

    "ProgramSettings":
    {
      "EntryPoints":
      [
        {
          "name": "MainVS",
          "type": "Vertex"
        },
        {
          "name": "MainPS",
          "type": "Fragment"
        }
      ]
    }
}
