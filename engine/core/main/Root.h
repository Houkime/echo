#pragma once

#include "engine/core/render/render/Renderer.h"
#include "engine/core/Resource/EchoThread.h"
#include "engine/core/main/EngineSettings.h"
#include "engine/core/main/EngineConsole.h"
#include "engine/core/Scene/NodeTree.h"
#include "FrameState.h"

namespace Echo
{
	class ProjectSettings;
	class ArchiveManager;
	class ArchiveFactory;
	class IO;
	class ImageCodecMgr;
	class RenderTargetManager;
	class ProfilerServer;
	class Root
	{	
	public:
		// 外部模块接口
		struct ExternalMgr
		{
			typedef std::function<void(int)> TickFun;
			typedef std::function<void()> ReleaseFun;
			typedef std::function<void()> RenderFun;

			String		m_name;			// 组件名称
			TickFun		m_tick;			// 更新函数 
			RenderFun	m_render;		// 渲染函数
			ReleaseFun	m_release;		// 释放函数
			bool		m_isReleased;	// 是否已经被释放
		};

		// 配置
		struct Config
		{
			typedef vector<ArchiveFactory*>::type ArchiveFactoryTypes;
			typedef vector<ExternalMgr>::type ExternalMgrs;

			String				projectFile;
			String				engineCfgFile;
			void*				pAssetMgr;					// for Android platfrom
			ArchiveFactoryTypes	externalArchiveFactorys;	// external archive factory
			int					m_AudiomaxVoribsCodecs;
			bool				m_AudioLoadDecompresse;
			bool				m_isEnableProfiler;			// 是否开启性能分析服务
			ExternalMgrs		m_externalMgrs;				// 外部模块

			unsigned int		m_windowHandle;
			bool				m_isGame;

			Config()
				: projectFile("")
				, engineCfgFile("engine.xml")
				, pAssetMgr(NULL)
				, m_AudiomaxVoribsCodecs(32)
				, m_AudioLoadDecompresse(false)
				, m_isEnableProfiler(false)
				, m_isGame(true)
			{}
		};

	public:
		// instance
		static Root* instance();

		void tick(i32 elapsedTime);

		float getFrameTime() { return m_frameTime; }

		// 是否已初始化
		bool isInited() const { return m_isInited; }

		// 装载日志系统
		bool initLogSystem();
		bool initialize(const Config& cfg);
		bool initRenderer(Renderer* pRenderer, const Renderer::RenderCfg& config);
		bool onRendererInited();

		// screen size changed
		bool onSize(ui32 width, ui32 height);
		void destroy();
		void releasePlugins();

		// 获取配置
		const Config& getConfig() const { return m_cfg; }

		// 获取资源主路径
		const String& getResPath() const;

		// 获取用户资源路径
		const String& getUserPath() const;

		// 设置用户资源路径
		void setUserPath(const String& strPath);

		void* getAssetManager() const;
		bool isRendererInited() const;
		const ui32&		getCurrentTime() const;

		inline void		setEnableFrameProfile( bool _enable ){ m_enableFrameProfile = _enable; }

		inline bool		getEnableFrameProfile() const { return m_enableFrameProfile; }

		FrameState&		frameState() { return m_frameState; }

		const FrameState& frameState() const { return m_frameState; }

		void resetFrameState() { m_frameState.reset(); }
		void SetPhoneinformation(int max,int free,String cpu);

		void setEnableFilterAdditional( bool _val) { m_enableFilterAdditional = _val; }

		bool getEnableFilterAdditional() const { return m_enableFilterAdditional; }

		// 设置帧缓冲缩放比
		void setFrameBufferScale( float scale) { m_framebufferScale =  scale; }
		
		// 获取帧缓存缩放比
		float getFramebufferScale() const { return m_framebufferScale; }
		
		void changeFilterAdditionalMap(const String& mapName);

		// 加载项目,引擎初始化时会自动调用，也可单独调用(全路径)
		void loadProject( const char* projectFile);
        
        void loadAllBankFile();

		// 当游戏挂起时候引擎需要进行的处理
		void onPlatformSuspend();

		// 当游戏从挂起中恢复时引擎需要进行的处理
		void onPlatformResume();

		// 设置资源延迟释放时间
		void setReleaseDelayTime(ui32 t);

	private:
		Root();
		~Root();

		// register all class types
		void registerClassTypes();

	public:
		ProjectSettings* getProjectFile() { return m_projectFile; }
		EngineSettingsMgr& getSettingsMgr() { return m_settingsMgr; }
		EngineConsole& getConsole() { return m_console; }

	protected:
		void updateAllManagerDelayResource();
		bool render();
		void loadLaunchScene();

	private:
		bool				m_isInited;				// 是否已初始化
		Config				m_cfg;					// 客户端传入
		EngineSettingsMgr	m_settingsMgr;			// 配置管理器
		EngineConsole		m_console;				// 命令行控制台
		String				m_resPath;				// 资源路径
		String				m_userPath;				// 用户资源路径
		void*				m_pAssetMgr;			// for android
		bool				m_bRendererInited;
		float				m_frameTime;
		ui32				m_currentTime;
		Renderer*			m_renderer;						// 渲染器
		bool				m_enableFrameProfile;
		FrameState			m_frameState;
		int					Maxmemory;
		int					Freememory;
		String				cputex;
		bool				m_enableBloom;
		bool				m_enableFilterAdditional;
		float				m_framebufferScale;             // 帧缓冲区缩放
		ProjectSettings*		m_projectFile;					// 项目信息

#ifdef ECHO_PROFILER
		ProfilerServer*		m_profilerSev;					// 性能分析服务器
#endif
	};
}

#define EchoRoot					Echo::Root::instance()
#define EchoMemoryManager			EchoRoot->getMemoryManager()
#define EchoEngineSettings			EchoRoot->getSettingsMgr()
#define EchoEngineConsole			EchoRoot->getConsole()
