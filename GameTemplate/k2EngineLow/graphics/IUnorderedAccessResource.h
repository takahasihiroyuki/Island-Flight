
namespace nsK2EngineLow {
	class IUnorderedAccessResource : public IShaderResource {
	public:
		virtual ~IUnorderedAccessResource() {};
		/// <summary>
		/// UAVÇ…ìoò^ÅB
		/// </summary>
		/// <param name=""></param>
		/// <param name="bufferNo"></param>
		virtual void RegisterUnorderedAccessView(D3D12_CPU_DESCRIPTOR_HANDLE, int bufferNo) = 0;
	};
}