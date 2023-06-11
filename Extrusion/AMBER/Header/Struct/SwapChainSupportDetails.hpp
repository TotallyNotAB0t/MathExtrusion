#ifndef __ENGINE_SWAP_CHAIN_SUPPORT_DETAILS__
#define __ENGINE_SWAP_CHAIN_SUPPORT_DETAILS__

#include "vulkan/vulkan.h"
#include <vector>

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;      //nombre min/max d'images dans la swap chain, hauteur/largeur min/max des images
    std::vector<VkSurfaceFormatKHR> formats;    //Format de la surface (format des pixels, palette de couleur)
    std::vector<VkPresentModeKHR> presentModes; //Mode de presentation disponibles
};

#endif //__ENGINE_SWAP_CHAIN_SUPPORT_DETAILS__