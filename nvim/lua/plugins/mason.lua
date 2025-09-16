return {
  {
    "williamboman/mason.nvim",
    cmd = "Mason",
    keys = {
      { "<leader>cm", "<cmd>Mason<cr>", desc = "Mason Installer" },
    },
    opts = {
      ensure_installed = {
        "lua-language-server",  -- Note: Correct package name
        "clangd",
        "stylua",
        "clang-format",
        "codelldb",
      },
    },
    config = function(_, opts)
      require("mason").setup(opts)

      -- Safe package installation
      local mr = require("mason-registry")
      for _, tool in ipairs(opts.ensure_installed) do
        pcall(function()
          local p = mr.get_package(tool)
          if not p:is_installed() then
            p:install()
          end
        end)
      end
    end,
  },
  {
    "williamboman/mason-lspconfig.nvim",
    dependencies = { "williamboman/mason.nvim" },
    opts = {
      automatic_installation = true,
      handlers = {
        function(server_name)
          require("lspconfig")[server_name].setup({})
        end,
      },
    },
  },
}
