#pragma once

#include <exception>
#include <optional>
#include <chrono>
#include <array>
#include <string>
#include <vector>
#include <set>
#include <cstdint> // Necessary for UINT32_MAX
#include <algorithm> // Necessary for std::min/std::max
#include <iostream>
#include <fstream>
#include <thread>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#define MakeErrorInfo(what) \
	ErrorInfo(what, __FILE__, std::to_string(__LINE__));

struct ErrorInfo
{
	std::string what;
	std::string file;
	std::string line;

	ErrorInfo(std::string what, std::string file, std::string line)
	{
		this->what = what;
		this->file = file;
		this->line = line;
	}
};

class ValidationLayers
{
public:
#ifdef _DEBUG
	static const bool enabled = true;
#else
	static const bool enabled = false;
#endif
	//See initialization in .cpp file
	static const std::vector<const char*> validationLayers;


	static bool checkSupport();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
};


static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoord;

	//Binding description
	//Tell Vulkan how to pass this data format to the vertex shader once it's been uploaded into GPU memory
	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0; //Index of the binding in the array of bindings
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	//Attribute descriptions
	//Describes how to handle vertex input
	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
		//The binding parameter tells Vulkan from which binding the per-vertex data comes.
		attributeDescriptions[0].binding = 0;
		//References the location directive of the input in the vertex shader
		attributeDescriptions[0].location = 0;
		//float: VK_FORMAT_R32_SFLOAT
		//vec2 : VK_FORMAT_R32G32_SFLOAT
		//vec3 : VK_FORMAT_R32G32B32_SFLOAT
		//vec4 : VK_FORMAT_R32G32B32A32_SFLOAT
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		//Specifies the number of bytes since the start of the per - vertex data to read from
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
		return attributeDescriptions;
	}
};

struct MVP_matrixes_t {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 projection;
};

class MainWindow
{
private:
	//The number of frames that can be rendered simultaneously
	const size_t				MAX_FRAMES_IN_FLIGHT = 2;
							   
	VkInstance					instance;
	VkDebugUtilsMessengerEXT	debugMessenger;
	VkPhysicalDevice			physicalDevice;
	struct QueueFamilyIndices {
		//Graphical operation support
		std::optional<uint32_t> graphicsFamily;
		//Drawing an image on a surface support
		std::optional<uint32_t> presentFamily;
		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};
	VkDevice					 logicalDevice;
	VkQueue						 graphicsQueue;
	VkQueue						 presentQueue;
	VkSurfaceKHR				 surface;
	GLFWwindow*					 window = nullptr;
	int							 windowWidth = 0;
	int							 windowHeight = 0;
	int							 windowXPos = 0;
	int							 windowYPos = 0;
	//List of required extensions
	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	struct SwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities{};
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
	VkSwapchainKHR				 swapchain;
	std::vector<VkImage>		 swapchainImages;
	VkFormat					 swapchainImageFormat{};
	VkExtent2D					 swapchainExtent{};
	std::vector<VkImageView>	 swapchainImageViews;
	VkRenderPass				 renderPass;
	VkDescriptorSetLayout		 descriptorSetLayout;
	VkPipelineLayout			 pipelineLayout;
	VkPipeline					 graphicsPipeline;
	std::vector<VkFramebuffer>	 swapchainFramebuffers;
	VkCommandPool				 commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	VkBuffer					 vertexBuffer;
	VkDeviceMemory				 vertexBufferMemory;
	VkBuffer					 indexBuffer;
	VkDeviceMemory				 indexBufferMemory;
	std::vector<VkBuffer>		 uniformBuffers;
	std::vector<VkDeviceMemory>  uniformBuffersMemory;
	VkDescriptorPool			 descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	VkImage						 textureImage;
	VkDeviceMemory				 textureImageMemory;
	VkImageView					 textureImageView;
	VkSampler					 textureSampler;

	std::vector<VkSemaphore>	 imageAvailableSemaphores;
	std::vector<VkSemaphore>	 renderFinishedSemaphores;
	std::vector<VkFence>		 inFlightFences;
	std::vector<VkFence>		 imagesInFlight;
	uint8_t						 currentFrame = 0;

	//imgui
	int							 minimalSwapchainImages = 0;
	VkDescriptorPool			 imguiDescriptorPool;
public:
	bool						 framebufferResized = false;
	bool						 minimizedWindowNeedClose = false;

public:
	void initVulkan();
	void renderLoop();
	void finishVulkan();
	void cleanupSwapchain();

	void recreateSwapchain();

	//Init
	void initGlfw();
	void createWindow();

	void createInstance();
	std::vector<const char*> getRequiredExtensions();

	void setupDebugMessenger();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	void createLogicalDevice();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	void createSurface();

	void createSwapchain();
	SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void createImageViews();
	VkImageView createImageView(VkImage image, VkFormat format);

	void createRenderPass();

	void createGraphicsPipeline();
	void createDescriptorSetLayout();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();

	std::vector<char> readShaderFile(const char* filePath);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	void createFramebuffers();

	void createCommandPool();

	//Allocates the command buffer from the command pool and starts recording, returns the allocated command buffer
	VkCommandBuffer beginSingleTimeCommands();
	//Finishes writing the command buffer and starts executing it, frees the command buffer
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	void createTextureImage();

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	void createTextureImageView();

	void createTextureSampler();

	void createVertexBuffer();
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void createIndexBuffer();

	void createCommandBuffers();

	void createSyncObjects();

	//imgui
	void imguiCreateDescriptorPool();
	void imguiInitImpl();

	//Render
	void drawFrame();
	void updateCommandBuffer(int imageIndex);
	void updateUniformBuffer(uint32_t currentImage);
};
