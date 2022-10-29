#include "MainWindow.h"

#include "imgui/imgui.h"
#include "impl_glfw/imgui_impl_glfw.h"
#include "impl_vulkan/imgui_impl_vulkan.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


#if defined(_WIN32)
    #undef min
	#undef max
	#if defined(__MINGW32__)
		#include <unistd.h>
	#endif
	#include <io.h>
	#include <process.h>
    #include <Windows.h>
    #include <shlwapi.h>
#elif defined(__APPLE_CC__)
    #include "ocutil.h"
#endif

#if defined(_WIN32)
    char* Unicode2Utf8(wchar_t* unicodeStr) {
        int cStrLen = WideCharToMultiByte(CP_UTF8, 0, unicodeStr, -1, NULL, 0, NULL, NULL);
        char* cStr = (char*)malloc(sizeof(char) * (cStrLen + 1));
        WideCharToMultiByte(CP_UTF8, 0, unicodeStr, -1, cStr, cStrLen + 1, NULL, NULL);
        *(cStr + cStrLen) = '\0';
        return cStr;
    }
#endif
std::string GetAssetsPath()
{
    std::string path;

    #if defined(_WIN32)
        wchar_t szBuf[512];
        ::GetModuleFileNameW(NULL, szBuf, 512);
        ::PathRemoveFileSpecW(szBuf);
        char* utf8 = Unicode2Utf8(szBuf);

        path.append(utf8);
        free(utf8); 

        std::replace(path.begin(), path.end(), '\\', '/');

    #elif defined(__APPLE_CC__)
        path = OCUtil_GetPathExecute();
        
    #endif
        std::cout << "Path exe: " << path << std::endl; 
        if (path[path.size() - 1] == '.')
        {
            path = path.substr(0, path.size() - 2);
        }
        size_t index = path.find_last_of('/');
        path = path.substr(0, index);
        path += "/";

    return path;
}


const std::vector<const char*> ValidationLayers::validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};


bool ValidationLayers::checkSupport()
{
    uint32_t layersCount = 0;
    //Get count
    vkEnumerateInstanceLayerProperties(&layersCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layersCount);
    //Get layers
    vkEnumerateInstanceLayerProperties(&layersCount, availableLayers.data());
    
    //Check layers support
    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const VkLayerProperties& avaiableLayerProperties : availableLayers) {
            if (strcmp(layerName, avaiableLayerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }
    return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL ValidationLayers::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {
    /*
    messageSeverity values
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: Diagnostic message
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT : Informational message like the creation of a resource
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT : Message about behavior that is not necessarily an error, but very likely a bug in your application
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT : Message about behavior that is invalidand may cause crashes
    */

    /*
    messageType values
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: Some event has happened that is unrelated to the specification or performance
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT : Something has happened that violates the specification or indicates a possible mistake
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT : Potential non - optimal use of Vulkan
    */

    std::string debugInfoStr = "";
    std::string messageSeverityStr = "";
    std::string messageTypeStr = "";
    std::string message = "";

    switch (messageSeverity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        messageSeverityStr = "Diagnostic";
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        messageSeverityStr = "Informational";
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        messageSeverityStr = "WARNING";
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        messageSeverityStr = "ERROR";
        break;
    default:
        messageSeverityStr = "Not recognized";
    }

    switch (messageType)
    {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        messageTypeStr = "Not specification/performance";
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
        messageTypeStr = "Specification violation or possible error";
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
        messageTypeStr = "Potential non-optimal use of Vulkan";
        break;
    default:
        messageTypeStr = "Not recognized";
    }

    message = pCallbackData->pMessage;

    debugInfoStr = "Validation layer\n"
        + (std::string)"Severity: " + messageSeverityStr + "\n"
        + (std::string)"Type: " + messageTypeStr + "\n"
        + (std::string)"Message: " + message + "\n\n";

    std::cout << debugInfoStr;

    return VK_FALSE;
}

const std::vector<Vertex> vertexes = {
	{{-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	{{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
};

static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

void MainWindow::initVulkan()
{
	initGlfw();
	createWindow();
	createInstance();
#ifdef _DEBUG
	setupDebugMessenger();
#endif // _DEBUG
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapchain();
	createImageViews();
	createRenderPass();
	createDescriptorSetLayout();
	createGraphicsPipeline();
	createFramebuffers();
	createCommandPool();
	createTextureImage();
	createTextureImageView();
	createTextureSampler();
	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
	createSyncObjects();

	//imgui
	imguiCreateDescriptorPool();
	imguiInitImpl();
}

void MainWindow::renderLoop()
{
	//Rendering cycle
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		drawFrame();
		if (minimizedWindowNeedClose) {
			return;
		}
	}
}

void MainWindow::finishVulkan()
{
	//To properly delete resources, they must be unoccupied
	vkDeviceWaitIdle(logicalDevice);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
	}
	cleanupSwapchain();
	vkDestroyDescriptorPool(logicalDevice, imguiDescriptorPool, nullptr);
	vkDestroySampler(logicalDevice, textureSampler, nullptr);
	vkDestroyImageView(logicalDevice, textureImageView, nullptr);
	vkDestroyImage(logicalDevice, textureImage, nullptr);
	vkFreeMemory(logicalDevice, textureImageMemory, nullptr);
	vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
	vkDestroyBuffer(logicalDevice, indexBuffer, nullptr);
	vkFreeMemory(logicalDevice, indexBufferMemory, nullptr);
	vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr);
	vkFreeMemory(logicalDevice, vertexBufferMemory, nullptr);

	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
	vkDestroyDevice(logicalDevice, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
#ifdef _DEBUG
	DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif
	vkDestroyInstance(instance, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();
}

void MainWindow::cleanupSwapchain()
{
	vkDeviceWaitIdle(logicalDevice);

	for (size_t i = 0; i < swapchainImages.size(); i++) {
		vkDestroyBuffer(logicalDevice, uniformBuffers[i], nullptr);
		vkFreeMemory(logicalDevice, uniformBuffersMemory[i], nullptr);
	}
	vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
	vkFreeCommandBuffers(logicalDevice, commandPool, commandBuffers.size(), commandBuffers.data());
	for (auto framebuffer : swapchainFramebuffers) {
		vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
	}
	vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
	vkDestroyRenderPass(logicalDevice, renderPass, nullptr);
	for (auto& imageView : swapchainImageViews) {
		vkDestroyImageView(logicalDevice, imageView, nullptr);
	}
	vkDestroySwapchainKHR(logicalDevice, swapchain, nullptr);
}

void MainWindow::recreateSwapchain()
{
	//For window minimization
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	windowWidth = width;
	windowHeight = height;
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
		if (glfwWindowShouldClose(window)) {
			//Closing a minimized window
			finishVulkan();
			minimizedWindowNeedClose = true;
			return;
		}
	}
	/*
	*
	* We first call vkDeviceWaitIdle,
	* because just like in the last chapter,
	* we shouldn't touch resources that may still be in use.
	* Obviously, the first thing we'll have to do is recreate the swap chain itself.
	* The image views need to be recreated because they are based directly on the swap chain images.
	* The render pass needs to be recreated because it depends on the format of the swap chain images.
	* It is rare for the swap chain image format to change during an operation like a window resize,
	* but it should still be handled.
	* Viewport and scissor rectangle size is specified during graphics pipeline creation,
	* so the pipeline also needs to be rebuilt.
	* It is possible to avoid this by using dynamic state for the viewports and scissor rectangles.
	* Finally, the framebuffers and command buffers also directly depend on the swap chain images.
	*
	*/
	vkDeviceWaitIdle(logicalDevice);

	cleanupSwapchain();

	createSwapchain();
	createImageViews();
	createRenderPass();
	createGraphicsPipeline();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createFramebuffers();
	createCommandBuffers();

	imagesInFlight.resize(swapchainImages.size(), VK_NULL_HANDLE);

	ImGui_ImplVulkan_SetMinImageCount(minimalSwapchainImages);
}

void MainWindow::initGlfw()
{
	//Initializing glfw
	if (!glfwInit()) {
		throw MakeErrorInfo("GLFW initialization failed!");
	}

	//glfw should not create any contexts
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	if (!glfwVulkanSupported()) {
		throw MakeErrorInfo("GLFW Vulkan not supported!");
	}
}

void MainWindow::createWindow()
{
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	if (primaryMonitor == NULL) {
		glfwTerminate();
		throw MakeErrorInfo("GLFW getting primary monitor failed!");
	}

	const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

	windowWidth = int(videoMode->width * 0.85);
	windowHeight = int(videoMode->height * 0.75);
	windowXPos = (videoMode->width - windowWidth) / 2;
	windowYPos = (videoMode->height - windowHeight) / 2;

	//Create window
	window = glfwCreateWindow(windowWidth, windowHeight, "Vulkan window", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		throw MakeErrorInfo("GLFW window creation failed!");
	}

	glfwSetWindowPos(window, windowXPos, windowYPos);

	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void MainWindow::createInstance()
{
	//Init Vulkan
	//Define app info(this is sometimes useful for the driver)
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Graphics Viewer";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	//for "Allow negative height to be specified in the VkViewport::height field to perform y-inversion of the clip-space to framebuffer-space transform.
	//This allows apps to avoid having to use gl_Position.y = -gl_Position.y in shaders also targeting other APIs." from VK_KHR_maintenance1 (Promotion to Vulkan 1.1)
	appInfo.apiVersion = VK_API_VERSION_1_1;

	//Here we can specify the extensions and validations layers required for us
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	//Get required extensions
	std::vector<const char*> extentionList = getRequiredExtensions();

	//Setting up extensions
	createInfo.enabledExtensionCount = extentionList.size();
	createInfo.ppEnabledExtensionNames = extentionList.data();

	//Setting up validation layers
	if (ValidationLayers::enabled && !ValidationLayers::checkSupport()) {
		throw MakeErrorInfo("Required validation layers are not available!");
	}
	if (ValidationLayers::enabled && ValidationLayers::checkSupport()) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers::validationLayers.size());
		createInfo.ppEnabledLayerNames = ValidationLayers::validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	if (result != VK_SUCCESS) {
		glfwDestroyWindow(window);
		glfwTerminate();
		throw MakeErrorInfo("VkInstance creation failed!");
	}
}

std::vector<const char*> MainWindow::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	//Requesting the necessary extensions to create an instance
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	//Add debug extention
	if (ValidationLayers::enabled) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void MainWindow::setupDebugMessenger()
{
	if (!ValidationLayers::enabled)
		return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	//Severity levels to be processed
	createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	//Types to be processed
	createInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	//Callback function
	createInfo.pfnUserCallback = ValidationLayers::debugCallback;
	createInfo.pUserData = nullptr; // Optional

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw MakeErrorInfo("Debug Utils Messenger creation failed!");
	}
}

VkResult MainWindow::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void MainWindow::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

void MainWindow::pickPhysicalDevice()
{
	//Get physical devices
	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
	if (physicalDeviceCount == 0) {
		throw MakeErrorInfo("Failed to find GPUs with Vulkan support!");
	}
	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

	//Check devices
	for (VkPhysicalDevice& device : physicalDevices) {
		if (isDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}
	if (physicalDevice == VK_NULL_HANDLE) {
		throw MakeErrorInfo("Failed to find a suitable GPU!");
	}
}

bool MainWindow::isDeviceSuitable(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	VkPhysicalDeviceFeatures supportedDeviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedDeviceFeatures);

	MainWindow::QueueFamilyIndices indices = findQueueFamilies(device);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapchainAdequate = false;
	if (extensionsSupported) {
		SwapchainSupportDetails swapchainSupport = querySwapchainSupport(device);
		swapchainAdequate = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
	}

	if (indices.isComplete() == false)
	{
		return false;
	}
	if (extensionsSupported == false)
	{
		return false;
	}
	if (swapchainAdequate == false)
	{
		return false;
	}
	if (supportedDeviceFeatures.samplerAnisotropy == false)
	{
		return false;
	}
	if (deviceProperties.limits.timestampComputeAndGraphics == false)
	{
		return false;
	}

	return true;
}

MainWindow::QueueFamilyIndices MainWindow::findQueueFamilies(VkPhysicalDevice device) {
	MainWindow::QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	for (size_t i = 0; i < queueFamilyCount; ++i) {
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}
	}

	return indices;
}

void MainWindow::createLogicalDevice()
{
	//Specifying the queues to be created
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		//We specify the data about the queues we are interested in and their number
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		//Queue priority(0.0 - 1.0) affects command buffer execution scheduling
		queueCreateInfo.pQueuePriorities = &queuePriority;

		queueCreateInfos.push_back(queueCreateInfo);
	}

	//Specifying used device features
	//Here we indicate the required device features that we want to use.
	//Nothing special is required right now.
	VkPhysicalDeviceFeatures deviceFeatures{};
	//Enable anisotropic filtering
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	//Creating the logical device
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = queueCreateInfos.size();

	createInfo.pEnabledFeatures = &deviceFeatures;

	//Set required extensions
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	createInfo.enabledExtensionCount = deviceExtensions.size();

	//Previous implementations of Vulkan made a distinction between instance and device specific validation layers,
	//but this is no longer the case.
	//That means that the enabledLayerCount and ppEnabledLayerNames fields of VkDeviceCreateInfo
	//are ignored by up-to-date implementations.
	//However, it is still a good idea to set them anyway to be compatible with older implementations
	if (ValidationLayers::enabled) {
		createInfo.enabledLayerCount = ValidationLayers::validationLayers.size();
		createInfo.ppEnabledLayerNames = ValidationLayers::validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to create logical device!");
	}

	//Get queue handler
	//Third parameter is index of queue, we created 1 queue from family, he index is 0
	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
	if (!graphicsQueue) {
		throw MakeErrorInfo("Failed to get graphics queue handler!");
	}
	vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
	if (!presentQueue) {
		throw MakeErrorInfo("Failed to get present queue handler!");
	}
}

bool MainWindow::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	//Get required extensions
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	//std::set is used to have a list of unsupported extensions
	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

void MainWindow::createSurface()
{
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to create window surface!");
	}
}

void MainWindow::createSwapchain()
{
	SwapchainSupportDetails swapchainSupport = querySwapchainSupport(physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapchainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapchainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapchainSupport.capabilities);

	//We have to choose the number of images in the swap chain queue that we want to use
	uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1; //using minimum means that we may sometimes have to wait on the driver to complete internal operations before we can acquire another image to render to.
	//if VkSurfaceCapabilitiesKHR::maxImageCount = 0, then there is no limit on images
	if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount) {
		imageCount = swapchainSupport.capabilities.maxImageCount;
	}

	//imgui
	minimalSwapchainImages = swapchainSupport.capabilities.minImageCount + 1;
	//ImGui_ImplVulkan_SetMinImageCount(minimalSwapchainImages);

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	//The imageArrayLayers specifies the amount of layers each image consists of. This is always 1 unless you are developing a stereoscopic 3D application.
	createInfo.imageArrayLayers = 1;
	//VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT specifies that the image can be used
	//to create a VkImageView suitable for use as a color or resolve attachment in a VkFramebuffer.
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	//Next, we need to specify how to handle swap chain images that will be used across multiple queue families.
	//That will be the case in our application if the graphics queue family is different from the presentation queue.
	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		//UNCOMMENT
		//createInfo.queueFamilyIndexCount = 0; // Optional
		//createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	//We will not apply transformations to images
	createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
	//The compositeAlpha field specifies if the alpha channel should be used for blending with other windows in the window system.
	//ignore alpha chanel
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.presentMode = presentMode;
	//If the "clipped" member is set to VK_TRUE then that means that we don't care about the color of pixels that are obscured,
	//for example because another window is in front of them.
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VkResult res = VK_SUCCESS;
	//if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
	res = vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapchain);
	if (res != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to create swapchain!");
	}

	//Get images
	vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, nullptr);
	swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, swapchainImages.data());

	swapchainImageFormat = surfaceFormat.format;
	swapchainExtent = extent;
}

MainWindow::SwapchainSupportDetails MainWindow::querySwapchainSupport(VkPhysicalDevice device)
{
	SwapchainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

VkSurfaceFormatKHR MainWindow::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats) {
		//Use SRGB
		//Check ARGB color and SRGB color space support
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
	}
	//Or use the first specified format
	return availableFormats[0];
}

VkPresentModeKHR MainWindow::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	//Use VK_PRESENT_MODE_MAILBOX_KHR
	//This mode can be considered triple buffering,
	//it allows you to have the most up-to-date images in the queue and avoid "image tearing".
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	//VK_PRESENT_MODE_FIFO_KHR mode is guaranteed to be available
	//Synchronized with the monitor frequency(?)
	return VK_PRESENT_MODE_FIFO_KHR;
}

uint32_t clamp(uint32_t v, uint32_t min, uint32_t max)
{
	if (v < min)
		return min;
	if (v > max)
		return max;
	return v;
}

VkExtent2D MainWindow::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	//The swap extent is the resolution of the swap chain images
	//and it's almost always exactly equal to the resolution of the window
	//that we're drawing to in pixels.

	if (capabilities.currentExtent.width != UINT32_MAX) {
		return capabilities.currentExtent;
	}
	else {
		int width = 0, height = 0;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

void MainWindow::createImageViews()
{
	swapchainImageViews.resize(swapchainImages.size());

	for (uint32_t i = 0; i < swapchainImages.size(); i++) {
		swapchainImageViews[i] = createImageView(swapchainImages[i], swapchainImageFormat);
	}
}

VkImageView MainWindow::createImageView(VkImage image, VkFormat format)
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(logicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to create texture image view!");
	}

	return imageView;
}

void MainWindow::createRenderPass()
{
	//Create render color attachment
	//Attachment description
	VkAttachmentDescription sceneRenderColorAttachment{};
	sceneRenderColorAttachment.format = swapchainImageFormat;
	sceneRenderColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	//Clear attachment before render
	sceneRenderColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	//The rendered attachment will be stored in memory for imgui render
	sceneRenderColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	//We are not interested in the stencil data
	sceneRenderColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	sceneRenderColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	//We are not interested in the layout before starting the render
	sceneRenderColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	//The layout should be VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL because the image will be drawn on top of this ImGui and only then displayed.
	sceneRenderColorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//Create imgui color attachment
	//Attachment description
	VkAttachmentDescription imguiColorAttachment{};
	imguiColorAttachment.format = swapchainImageFormat;
	imguiColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	//Load render attachment before render imgui
	imguiColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	//Render attachment will be stored in memory for visualization
	imguiColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	//We are not interested in the stencil data
	imguiColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	imguiColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	imguiColorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	//We want the image to be ready for presentation using the swap chain after rendering
	imguiColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	std::vector<VkAttachmentDescription> attachmentsDescriptions = { sceneRenderColorAttachment, imguiColorAttachment };

	//Subpasses and attachment references
	//Subpass refers to the attachment using VkAttachmentReference
	VkAttachmentReference sceneRenderColorAttachmentRef{};
	//Index of the attachment that we created above earlier
	sceneRenderColorAttachmentRef.attachment = 0;
	sceneRenderColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference imguiColorAttachmentRef{};
	imguiColorAttachmentRef.attachment = 1;
	imguiColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//std::vector<VkAttachmentReference> attachmentsReferences = { sceneRenderColorAttachmentRef, imguiColorAttachmentRef };

	VkSubpassDescription sceneRenderSubpass{};
	sceneRenderSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	sceneRenderSubpass.colorAttachmentCount = 1;
	//The index of the attachment in this array is directly referenced from the fragment shader
	//with the layout(location = 0) out vec4 outColor directive!
	sceneRenderSubpass.pColorAttachments = &sceneRenderColorAttachmentRef;

	VkSubpassDescription imguiSubpass{};
	imguiSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	imguiSubpass.colorAttachmentCount = 1;
	imguiSubpass.pColorAttachments = &imguiColorAttachmentRef;

	std::vector<VkSubpassDescription> subpasses = { sceneRenderSubpass, imguiSubpass };

	//Subpass dependencies
	//Remember that the subpasses in a render pass automatically take care of image layout transitions.
	//These transitions are controlled by subpass dependencies,
	//which specify memory and execution dependencies between subpasses.
	//We have only a single subpass right now,
	//but the operations right before and right after this subpass also count as implicit "subpasses".
	VkSubpassDependency sceneRenderSubpassDependency{};
	sceneRenderSubpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	sceneRenderSubpassDependency.dstSubpass = 0;
	sceneRenderSubpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	sceneRenderSubpassDependency.srcAccessMask = 0;
	sceneRenderSubpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	sceneRenderSubpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkSubpassDependency imguiSubpassDependency{};
	imguiSubpassDependency.srcSubpass = 0;
	imguiSubpassDependency.dstSubpass = 1;
	imguiSubpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	imguiSubpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	imguiSubpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	imguiSubpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::vector<VkSubpassDependency> subpassesDependencies = { sceneRenderSubpassDependency, imguiSubpassDependency };

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = attachmentsDescriptions.size();
	renderPassInfo.pAttachments = attachmentsDescriptions.data();
	renderPassInfo.subpassCount = subpasses.size();
	renderPassInfo.pSubpasses = subpasses.data();
	renderPassInfo.dependencyCount = subpassesDependencies.size();
	renderPassInfo.pDependencies = subpassesDependencies.data();

	if (vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to create renderpass!");
	}
}

void MainWindow::createGraphicsPipeline()
{
	std::string pathVertexShader = GetAssetsPath() + "Assets/Shader/Vulkan/pos3_color3_tex2_ubo.vert.spv";
	std::vector<char> vertShaderCode = readShaderFile(pathVertexShader.c_str());
	std::string pathFragmentShader = GetAssetsPath() + "Assets/Shader/Vulkan/pos3_color3_tex2_ubo.frag.spv";
	std::vector<char> fragShaderCode = readShaderFile(pathFragmentShader.c_str());

	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	//Shader entrypoint
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	std::vector<VkPipelineShaderStageCreateInfo> shaderStages = { vertShaderStageInfo, fragShaderStageInfo };

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	//Describes the format of the vertex data that will be passed to the vertex shader.
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	//Bindings: spacing between data and whether the data is per-vertex or per-instance
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	//Attribute descriptions: type of the attributes passed to the vertex shader,
	//which binding to load them fromand at which offset
	vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	//Describes two things:
	//What kind of geometry will be drawn from the vertices and if primitive restart should be enabled.
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	//This parameter points to something similar to "element buffer"
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = (float)swapchainExtent.height;
	viewport.width = (float)swapchainExtent.width;
	viewport.height = -(float)swapchainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	//The framebuffer content that goes beyond the scissors will be discarded
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapchainExtent;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	//Setting up the rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	//If depthClampEnable is set to VK_TRUE,
	//then fragments that are beyond the nearand far planes are clamped to them as opposed to discarding them.
	//This is useful in some special cases like shadow maps.Using this requires enabling a GPU feature.
	rasterizer.depthClampEnable = VK_FALSE;
	//If rasterizerDiscardEnable is set to VK_TRUE, then geometry never passes through the rasterizer stage.
	//This basically disables any output to the framebuffer.
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	//Fill/lines/vertexes points
	//Using any mode other than fill requires enabling a GPU feature.
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	//The back faces will be discarded
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

	//The rasterizer can alter the depth values by adding a constant value or biasing them based on a fragment's slope.
	//This is sometimes used for shadow mapping.
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	//Setting up multisampling (anti-aliasing)
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	//Setting up color blending

	//This per-framebuffer struct allows you to configure the first way of color blending.
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	//Setting up dynamic states
	std::vector<VkDynamicState> dynamicStates = {
		//VK_DYNAMIC_STATE_VIEWPORT,
		//VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = dynamicStates.size();
	dynamicState.pDynamicStates = dynamicStates.data();

	//Pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1; // Optional
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to create pipeline layout!");
	}

	//Creating graphics pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState; // Optional
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	if (vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to create graphics pipeline!");
	}

	vkDestroyShaderModule(logicalDevice, fragShaderModule, nullptr);
	vkDestroyShaderModule(logicalDevice, vertShaderModule, nullptr);
}

void MainWindow::createDescriptorSetLayout()
{
	std::vector<VkDescriptorSetLayoutBinding> uboLayoutBindings(2);
	//MVP_matrixes
	uboLayoutBindings[0].binding = 0; //It's like location
	uboLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBindings[0].descriptorCount = 1;
	uboLayoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBindings[0].pImmutableSamplers = nullptr; // Optional
	//Texture sampler
	uboLayoutBindings[1].binding = 1;
	//This descriptor makes it possible for shaders to access an image resource through a sampler object like
	uboLayoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	uboLayoutBindings[1].descriptorCount = 1;
	uboLayoutBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	uboLayoutBindings[1].pImmutableSamplers = nullptr; // Optional

	//All of the descriptor bindings are combined into a single VkDescriptorSetLayout object.
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = uboLayoutBindings.size();
	layoutInfo.pBindings = uboLayoutBindings.data();

	if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to create descriptor set layout!");
	}
}

void MainWindow::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(MVP_matrixes_t);

	uniformBuffers.resize(swapchainImages.size());
	uniformBuffersMemory.resize(swapchainImages.size());

	for (size_t i = 0; i < swapchainImages.size(); i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void MainWindow::createDescriptorPool()
{
	//Descriptor SETS can't be created directly, they must be allocated from a POOL

	//Descriptor SET info
	std::vector<VkDescriptorPoolSize> poolSizes(2);
	//MVP_matrixes
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(swapchainImages.size());
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(swapchainImages.size());

	//Descriptor POOL info
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = poolSizes.size();
	poolInfo.pPoolSizes = poolSizes.data();
	//Aside from the maximum number of individual descriptors that are available,
	//we also need to specify the maximum number of descriptor sets that may be allocated
	poolInfo.maxSets = swapchainImages.size();

	if (vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to create descriptor pool!");
	}
}

void MainWindow::createDescriptorSets()
{
	//A descriptor set allocation is described with a VkDescriptorSetAllocateInfo struct.
	//You need to specify the descriptor pool to allocate from, the number of descriptor sets to allocate,
	//and the descriptor layout to base them on
	descriptorSets.resize(swapchainImages.size());

	std::vector<VkDescriptorSetLayout> layouts(swapchainImages.size(), descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapchainImages.size());
	allocInfo.pSetLayouts = layouts.data();

	if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to allocate descriptor sets!");
	}

	//The descriptor sets have been allocated now, but the descriptors within still need to be configured
	//Descriptors that refer to buffers, like our uniform buffer descriptor,
	//are configured with a VkDescriptorBufferInfo struct.
	for (size_t i = 0; i < swapchainImages.size(); i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		//region within buffer that contains the data for the descriptor.
		bufferInfo.range = sizeof(MVP_matrixes_t);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;

		//The configuration of descriptors is updated using the vkUpdateDescriptorSets function,
		//which takes an array of VkWriteDescriptorSet structs as parameter.
		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		//Remember that descriptors can be arrays,
		//so we also need to specify the first index in the array that we want to update.
		//We're not using an array, so the index is simply 0.
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		//It's possible to update multiple descriptors at once in an array, starting at index dstArrayElement.
		//The descriptorCount field specifies how many array elements you want to update.
		descriptorWrites[0].descriptorCount = 1;
		//pImageInfo is used for descriptors that refer to image data,
		//and pTexelBufferView is used for descriptors that refer to buffer views.
		//Our descriptor is based on buffers, so we're using pBufferInfo.
		descriptorWrites[0].pBufferInfo = &bufferInfo;
		descriptorWrites[0].pImageInfo = nullptr; // Optional
		descriptorWrites[0].pTexelBufferView = nullptr; // Optional

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(logicalDevice, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
	}
}

std::vector<char> MainWindow::readShaderFile(const char* filePath)
{
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		throw MakeErrorInfo("Failed to open shader file!");
	}
	size_t file_size = (size_t)file.tellg();
	file.seekg(0);
	std::vector<char> fileData(file_size);
	file.read(fileData.data(), file_size);
	file.close();
	return fileData;
}

VkShaderModule MainWindow::createShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
	VkShaderModule shaderModule;
	if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to create shader module!");
	}
	return shaderModule;
}

void MainWindow::createFramebuffers()
{
	swapchainFramebuffers.resize(swapchainImageViews.size());

	//Creating a framebuffer for each image
	for (size_t i = 0; i < swapchainImageViews.size(); i++) {
		std::vector<VkImageView> framebufferAttachments = { swapchainImageViews[i], swapchainImageViews[i] };

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = framebufferAttachments.size();
		framebufferInfo.pAttachments = framebufferAttachments.data();
		framebufferInfo.width = swapchainExtent.width;
		framebufferInfo.height = swapchainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &swapchainFramebuffers[i]) != VK_SUCCESS) {
			throw MakeErrorInfo("Failed to create framebuffer!");
		}
	}
}

void MainWindow::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	if (vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to create command pool!");
	}
}

VkCommandBuffer MainWindow::beginSingleTimeCommands()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void MainWindow::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VkFenceCreateInfo createFenceInfo{};
	createFenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	VkFence executionCompleteFence;
	vkCreateFence(logicalDevice, &createFenceInfo, nullptr, &executionCompleteFence);

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, executionCompleteFence);

	vkWaitForFences(logicalDevice, 1, &executionCompleteFence, VK_TRUE, UINT64_MAX);
	vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
	vkDestroyFence(logicalDevice, executionCompleteFence, nullptr);
}

void MainWindow::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	//The extent field specifies the dimensions of the image, basically how many texels there are on each axis.
	//That's why depth must be 1 instead of 0. 
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	//It is possible that the format is not supported by the graphics hardware.
	//You should have a list of acceptable alternativesand go with the best one that is supported.
	//However, support for this particular format is so widespread that we'll skip this step.
	//Using different formats would also require annoying conversions.
	imageInfo.format = format;
	//If you want to be able to directly access texels in the memory of the image,
	//then you must use VK_IMAGE_TILING_LINEAR.
	//We will be using a staging buffer instead of a staging image,
	//so this won't be necessary. We will be using VK_IMAGE_TILING_OPTIMAL for efficient access from the shader.
	imageInfo.tiling = tiling;
	//VK_IMAGE_LAYOUT_UNDEFINED: Not usable by the GPUand the very first transition will discard the texels.
	//VK_IMAGE_LAYOUT_PREINITIALIZED: Not usable by the GPU, but the first transition will preserve the texels.
	//There are few situations where it is necessary for the texels to be preserved during the first transition.
	//One example, however, would be if you wanted to use an image as a staging image in combination with the VK_IMAGE_TILING_LINEAR layout.
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	//The image will only be used by one queue family: the one that supports graphics(and therefore also) transfer operations.	
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	//The samples flag is related to multisampling.
	//This is only relevant for images that will be used as attachments, so stick to one sample.
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0; // Optional

	if (vkCreateImage(logicalDevice, &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(logicalDevice, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to allocate image memory!");
	}

	vkBindImageMemory(logicalDevice, image, imageMemory, 0);
}

void MainWindow::createTextureImage()
{
	int textureImageWidth, textureImageHeight, textureImageChannels;
	std::string pathTexture = GetAssetsPath() + "Assets/Texture/texture.png";
	stbi_uc* textureImagePixels = stbi_load(pathTexture.c_str(), &textureImageWidth, &textureImageHeight, &textureImageChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = (uint64_t)textureImageWidth * textureImageHeight * 4;

	if (!textureImagePixels) {
		throw MakeErrorInfo("Failed to load texture image!");
	}

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	createBuffer(imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer,
		stagingBufferMemory);

	//Copy image data to buffer
	void* dataPtr;
	vkMapMemory(logicalDevice, stagingBufferMemory, 0, imageSize, 0, &dataPtr);
	memcpy(dataPtr, textureImagePixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(logicalDevice, stagingBufferMemory);

	stbi_image_free(textureImagePixels);

	createImage(textureImageWidth,
		textureImageHeight,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		textureImage,
		textureImageMemory);

	//The next step is to copy the staging buffer to the texture image. This involves two steps:
	//1. Transition the texture image to VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
	//2. Execute the buffer to image copy operation
	transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(textureImageWidth), static_cast<uint32_t>(textureImageHeight));

	//To be able to start sampling from the texture image in the shader, we need one last transition to prepare it for shader access:
	transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

void MainWindow::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkBufferImageCopy region{};
	//The bufferOffset specifies the byte offset in the buffer at which the pixel values start.
	//The bufferRowLength and bufferImageHeight fields specify how the pixels are laid out in memory.
	//For example, you could have some padding bytes between rows of the image.
	//Specifying 0 for both indicates that the pixels are simply tightly packed like they are in our case. 
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	//The imageSubresource, imageOffset and imageExtent fields indicate to which part of the image we want to copy the pixels.
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};

	vkCmdCopyBufferToImage(
		commandBuffer,
		buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, //Indicates which layout the image is currently using.
											  //Assuming here that the image has already been transitioned to the layout that is optimal for copying pixels to.
		1,
		&region
	);

	endSingleTimeCommands(commandBuffer);
}

void MainWindow::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	//If you are using the barrier to transfer queue family ownership, then these two fields should be the indices of the queue families.
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	//The image and subresourceRange specify the image that is affected and the specific part of the image.
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	//Barriers are primarily used for synchronization purposes,
	//so you must specify which types of operations that involve the resource must happen before the barrier,
	//and which operations that involve the resource must wait on the barrier.
	//barrier.srcAccessMask = 0;
	//barrier.dstAccessMask = 0;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	//TABLE
	//https://www.khronos.org/registry/vulkan/specs/1.0/html/vkspec.html#synchronization-access-types-supported
	//Undefined -> transfer destination: transfer writes that don't need to wait on anything
	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT; //All host write operations are performed before the barrier
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; //All write access to an image or buffer in a clear or copy operations are performed after the barrier

		//barrier.srcAccessMask = VK_ACCESS_NONE_KHR; //Not used before the barrier
		//barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; //Write access to an image or buffer in a clear or copy operation

		//One thing to note is that command buffer submission results in implicit VK_ACCESS_HOST_WRITE_BIT synchronization at the beginning.
		//Since the transitionImageLayout function executes a command buffer with only a single command,
		//you could use this implicit synchronizationand set srcAccessMask to 0 if you ever needed a VK_ACCESS_HOST_WRITE_BIT dependency in a layout transition.
		//It's up to you if you want to be explicit about it or not, but I'm personally not a fan of relying on these OpenGL - like "hidden" operations.

		sourceStage = VK_PIPELINE_STAGE_HOST_BIT; //See table
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT; //See table
	}
	//Transfer destination -> shader reading: shader reads should wait on transfer writes, specifically the shader reads in the fragment shader, because that's where we're going to use the texture
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		//for transfer destination -> shader reading
		//Now in this case, there is a binding to the fragment shader, but what if you need to use a different shader?
		//For example, if the image will be used in another shader.
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT; //See table
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		throw MakeErrorInfo("Unsupported layout transition!")
	}

	vkCmdPipelineBarrier(
		commandBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	endSingleTimeCommands(commandBuffer);

	//There is actually a special type of image layout that supports all operations, VK_IMAGE_LAYOUT_GENERAL.
	//The problem with it, of course, is that it doesn't necessarily offer the best performance for any operation. It is required for some special cases,
	//like using an image as both input and output, or for reading an image after it has left the preinitialized layout.
}

void MainWindow::createTextureImageView()
{
	textureImageView = createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB);
}

void MainWindow::createTextureSampler()
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	//The addressing mode can be specified per axis using the addressMode fields.
	//The available values are listed below.Most of these are demonstrated in the image above.
	//Note that the axes are called U, Vand W instead of X, Yand Z.This is a convention for texture space coordinates.
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	//Anisotropic filtering
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(physicalDevice, &properties);
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	//The borderColor field specifies which color is returned when sampling beyond the image with clamp to border(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER) addressing mode.
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	//The unnormalizedCoordinates field specifies which coordinate system you want to use to address texels in an image.
	//If it is VK_TRUE, then you can simply use coordinates within the [0, texWidth) and [0, texHeight) range.
	//If it is VK_FALSE, then the texels are addressed using the [0, 1) range on all axes.
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	//If a comparison function is enabled, then texels will first be compared to a value, and the result of that comparison is used in filtering operations. 
	//This is mainly used for percentage-closer filtering on shadow maps.
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	//Mipmap settings
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(logicalDevice, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to create texture sampler!");
	}

	//Note the sampler does not reference a VkImage anywhere.
	//The sampler is a distinct object that provides an interface to extract colors from a texture.
	//It can be applied to any image you want, whether it is 1D, 2D or 3D.This is different from many older APIs, which combined texture imagesand filtering into a single state.
}

void MainWindow::createVertexBuffer()
{
	//Memory type(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) that allows us to access it from the CPU
	//may not be the most optimal memory type for the graphics card itself to read from.
	//The most optimal memory has the VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT flag
	//and is usually not accessible by the CPU on dedicated graphics cards.
	//We're going to create two vertex buffers.
	//One staging buffer in CPU accessible memory to upload the data from the vertex array to,
	//and the final vertex buffer in device local memory.
	//We'll then use a buffer copy command to move the data from the staging buffer to the actual vertex buffer.

	VkDeviceSize bufferSize = (VkDeviceSize)sizeof(vertexes[0]) * vertexes.size();

	//Create staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	//VK_BUFFER_USAGE_TRANSFER_SRC_BIT: Buffer can be used as source in a memory transfer operation
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	//Filling the staging buffer
	void* dataPtr;
	//Map a memory object into application address space
	vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &dataPtr);
	memcpy(dataPtr, vertexes.data(), (size_t)bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferMemory);

	//Create vertex buffer
	//VK_BUFFER_USAGE_TRANSFER_DST_BIT: Buffer can be used as destination in a memory transfer operation
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

void MainWindow::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to create vertex buffer!");
	}

	//The buffer has been created,
	//but it doesn't actually have any memory assigned to it yet.
	//The first step of allocating memory for the buffer is to query its memory requirements
	//using vkGetBufferMemoryRequirements function.

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	//Allocate device memory
	if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to allocate vertex buffer memory!");
	}

	//Bind device memory to a buffer object
	vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);
}

uint32_t MainWindow::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	//Query info about the available types of memory
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	//Find a memory type that is suitable for the buffer itself:
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}
	throw MakeErrorInfo("Failed to find memory type");
}

void MainWindow::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	endSingleTimeCommands(commandBuffer);
}

void MainWindow::createIndexBuffer()
{
	VkDeviceSize bufferSize = (VkDeviceSize)sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

	copyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

void MainWindow::createCommandBuffers()
{
	commandBuffers.resize(swapchainFramebuffers.size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	//Allocate command buffers from command pool
	if (vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to allocate command buffers!");
	}
}

void MainWindow::createSyncObjects()
{
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(swapchainImages.size(), VK_NULL_HANDLE);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw MakeErrorInfo("Failed to create synchronization objects for a frame!");
		}
	}
}

//imgui
void checkImguiError(VkResult err)
{
}

void MainWindow::imguiCreateDescriptorPool()
{
	VkDescriptorPoolSize pool_sizes[] = {
	{VK_DESCRIPTOR_TYPE_SAMPLER,                1000},
	{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
	{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000},
	{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000},
	{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000},
	{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000},
	{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000},
	{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000},
	{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
	{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
	{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000}
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
	pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;

	if (vkCreateDescriptorPool(logicalDevice, &pool_info, nullptr, &imguiDescriptorPool) != VK_SUCCESS) {
		throw MakeErrorInfo("imgui descriptor pool creation failed!");
	}
}

void MainWindow::imguiInitImpl()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();// (void)io;

	io.IniFilename = NULL; //Disable imgui.ini file
	//io.ConfigFlags
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	ImGui_ImplGlfw_InitForVulkan(window, true);

	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = instance;
	init_info.PhysicalDevice = physicalDevice;
	init_info.Device = logicalDevice;
	init_info.QueueFamily = findQueueFamilies(physicalDevice).graphicsFamily.value();
	init_info.Queue = graphicsQueue;
	init_info.DescriptorPool = imguiDescriptorPool;
	init_info.MinImageCount = minimalSwapchainImages;
	init_info.ImageCount = swapchainImages.size();
	init_info.CheckVkResultFn = checkImguiError;
	init_info.Subpass = 1;
	ImGui_ImplVulkan_Init(&init_info, renderPass);

	VkCommandBuffer commandBuffer = beginSingleTimeCommands();
	ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
	endSingleTimeCommands(commandBuffer);
	ImGui_ImplVulkan_DestroyFontUploadObjects();
}

//Render
void MainWindow::drawFrame()
{
	//Waiting for the fence to be ready(Waiting for the command buffer to finish executing for the frame)
	vkWaitForFences(logicalDevice, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	//Acquire image from swapchain
	uint32_t imageIndex;
	//UINT64_MAX disables timeout, timeout is infinite
	VkResult result = vkAcquireNextImageKHR(logicalDevice, swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapchain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw MakeErrorInfo("Failed to acquire swapchain image!");
	}

	// Check if a previous frame is using this image (i.e. there is its fence to wait on)
	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(logicalDevice, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}

	// Mark the image as now being in use by this frame
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];

	glfwGetWindowPos(window, &windowXPos, &windowYPos);

	updateCommandBuffer(imageIndex);

	updateUniformBuffer(imageIndex);

	//Submitting the command buffer
	//Each entry in the waitStages array corresponds to the semaphore with the same index in pWaitSemaphores.
	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	std::vector<VkPipelineStageFlags> waitStages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//The first three parameters specify which semaphores to wait on before execution begins
	//and in which stage(s) of the pipeline to wait.
	//We want to wait with writing colors to the image until it's available, so we're specifying the stage of the graphics pipeline that writes to the color attachment.
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages.data();
	std::vector<VkCommandBuffer> currentCommandBuffers = { commandBuffers[imageIndex] };
	submitInfo.commandBufferCount = currentCommandBuffers.size();
	submitInfo.pCommandBuffers = currentCommandBuffers.data();
	std::vector<VkSemaphore> signalSemaphores = { renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = signalSemaphores.size();
	submitInfo.pSignalSemaphores = signalSemaphores.data();

	//Restore the fence to the unsignaled state
	vkResetFences(logicalDevice, 1, &inFlightFences[currentFrame]);

	//Start executing the command buffer
	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = signalSemaphores.size();
	presentInfo.pWaitSemaphores = signalSemaphores.data();

	VkSwapchainKHR swapchains[] = { swapchain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapchains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	result = vkQueuePresentKHR(presentQueue, &presentInfo);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		recreateSwapchain();
		if (minimizedWindowNeedClose) {
			return;
		}
	}
	else if (result != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to present swapchain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void MainWindow::updateCommandBuffer(int imageIndex)
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	static bool windowOpened = true;
	static bool showDemoWindow = false;
	ImGui::Begin("Rendertime", &windowOpened, 0);
	ImGui::Text("Frametime: %f", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Checkbox("Show ImGui demo window", &showDemoWindow);
	ImGui::End();
	if (showDemoWindow) {
		ImGui::ShowDemoWindow();
	}

	ImGui::Render();
	ImDrawData* main_draw_data = ImGui::GetDrawData();

	if (vkResetCommandBuffer(commandBuffers[imageIndex], 0) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to reset command buffer!");
	}

	//Starting recording command buffer
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	//Start recording
	if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to begin recording command buffer!");
	}

	//Starting a render pass
	//Configure render pass
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = swapchainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapchainExtent;

	VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	//Draw square
	vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffers[imageIndex], 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffers[imageIndex], indexBuffer, 0, VK_INDEX_TYPE_UINT16);
	vkCmdBindDescriptorSets(commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[imageIndex], 0, nullptr);

	vkCmdDrawIndexed(commandBuffers[imageIndex], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
	
	//Draw ImGui
	vkCmdNextSubpass(commandBuffers[imageIndex], VK_SUBPASS_CONTENTS_INLINE);
	ImGui_ImplVulkan_RenderDrawData(main_draw_data, commandBuffers[imageIndex]);
	
	vkCmdEndRenderPass(commandBuffers[imageIndex]);
	if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
		throw MakeErrorInfo("Failed to record command buffer!");
	}
}

void MainWindow::updateUniformBuffer(uint32_t currentImage)
{
	//We'll use chrono to make sure that the geometry rotates 90 degrees per second regardless of frame rate.
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	MVP_matrixes_t mvp_matrixes;

	//Valid if
	//VkViewport::height negative
	//VkViewport::y = swapchain height
	//GLM_FORCE_LEFT_HANDED defined
	//Moves deeper into the screen.
	//Rotates counterclockwise on the screen.
	//The scale does not break the matrices.
	//mvp_matrixes.model = 
	//	glm::translate(glm::mat4(1.0f), glm::vec3(time * 0.0, time * 0.0f, time * 0.5f)) *
	//	glm::rotate(glm::mat4(1.0f), time * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
	//	glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5, 1.0));
	//mvp_matrixes.view = glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//mvp_matrixes.projection = glm::perspective(glm::radians(45.0f), float(swapchainExtent.width) / (float)swapchainExtent.height, 0.1f, 10.0f);
	mvp_matrixes.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	mvp_matrixes.view = glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mvp_matrixes.projection = glm::perspective(glm::radians(45.0f), float(swapchainExtent.width) / (float)swapchainExtent.height, 0.1f, 10.0f);

	void* dataPtr;
	vkMapMemory(logicalDevice, uniformBuffersMemory[currentImage], 0, sizeof(MVP_matrixes_t), 0, &dataPtr);
	memcpy(dataPtr, &mvp_matrixes, sizeof(mvp_matrixes));
	vkUnmapMemory(logicalDevice, uniformBuffersMemory[currentImage]);
}