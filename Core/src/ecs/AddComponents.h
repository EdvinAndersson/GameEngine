#pragma once
#include "GameObject.h"
namespace CW {
  void AddC(GameObject obj) {
      obj.AddComponent<ModelRenderer>();
  }
}
