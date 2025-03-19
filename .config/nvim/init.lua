-- AstroNvim config for DIPAL project
-- This file can be symlinked to ~/.config/nvim/init.lua or used with NVIM_APPNAME
-- Example: NVIM_APPNAME=dipal nvim .

-- You can use this to configure AstroNvim specifically for this C++ project
-- if you want to use project-specific settings

-- Load base AstroNvim configuration
local astro_path = vim.fn.stdpath("config") .. "/lua/astro"
if vim.fn.filereadable(astro_path) == 1 then
  dofile(astro_path)
else
  -- If not running a local instance, assume normal AstroNvim config is available
  -- This allows you to use this file with NVIM_APPNAME=dipal nvim
  -- or just reference it in your normal AstroNvim config
end

-- Project-specific settings
local config = {
  -- Configure C++ specific options
  lsp = {
    -- Setup C++ lsp servers
    servers = {
      "clangd",
      "cmake",
    },
    -- Configure clangd
    config = {
      clangd = {
        cmd = {
          "clangd",
          "--background-index",
          "--suggest-missing-includes",
          "--clang-tidy",
          "--header-insertion=iwyu",
        },
      },
    },
  },
  -- Configure diagnostics
  diagnostics = {
    underline = true,
    update_in_insert = false,
    virtual_text = { spacing = 4, prefix = "●" },
    severity_sort = true,
  },
  -- Configure formatting
  formatting = {
    format_on_save = {
      enabled = true,
      allow_filetypes = { "cpp", "hpp", "cmake" },
    },
    disabled = {},
    timeout_ms = 1000,
  },
  -- Configure treesitter
  treesitter = {
    ensure_installed = {
      "c",
      "cpp",
      "cmake",
      "lua",
      "markdown",
      "markdown_inline",
    },
  },
}

-- Return the configuration if being imported or apply if run directly
return config
