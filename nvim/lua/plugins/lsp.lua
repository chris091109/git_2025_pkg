return {
  {
    "neovim/nvim-lspconfig",
    event = { "BufReadPre", "BufNewFile" },
    dependencies = {
      "williamboman/mason.nvim",
      "williamboman/mason-lspconfig.nvim",
      "hrsh7th/cmp-nvim-lsp", -- Add this dependency
    },
    config = function()
      -- Enable LSP capabilities for nvim-cmp
      local capabilities = require("cmp_nvim_lsp").default_capabilities()

      -- C/C++ (clangd)
      require("lspconfig").clangd.setup({
        capabilities = capabilities, -- Add this line
      })

      -- Lua (lua_ls)
      require("lspconfig").lua_ls.setup({
        capabilities = capabilities, -- Add this line
        settings = {
          Lua = {
            runtime = { version = "LuaJIT" },
            diagnostics = { globals = { "vim" } },
            workspace = { library = vim.api.nvim_get_runtime_file("", true) },
          },
        },
      })
    end,
  }
}
