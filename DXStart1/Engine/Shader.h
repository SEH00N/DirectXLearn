#pragma once
class Shader
{
public:
    void Init(const wstring& path);
    void Update();

private:
    void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
    void CreateVertexShader(const wstring& path, const string& name, const string& version);
    void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
    ComPtr<ID3DBlob>                    vertexShaderBlob;
    ComPtr<ID3DBlob>                    pixelShaderBlob;
    ComPtr<ID3DBlob>                    errBlob;

    ComPtr<ID3D12PipelineState>             pipelineState;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC      pipelineDesc = {};
};

