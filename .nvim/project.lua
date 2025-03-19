-- AstroNvim project-specific configuration
return {
  -- lsp configs
  lsp = {
    -- formatter
    formatting = {
      -- control auto formatting on save
      format_on_save = {
        enabled = true,
        allow_filetypes = {
          "cpp",
          "hpp",
          "c",
          "h",
          "cmake",
        },
      },
    },
  },
  -- diagnostics configuration
  diagnostics = {
    underline = true,
    virtual_text = true,
  },
}
