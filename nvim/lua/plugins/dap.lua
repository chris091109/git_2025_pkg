return {
  -- Debugger core
  {
    "mfussenegger/nvim-dap",
    lazy = true,
    keys = {
      { "<F5>", function() require("dap").continue() end, desc = "Debug: Start/Continue" },
      { "<F10>", function() require("dap").step_over() end, desc = "Debug: Step Over" },
      { "<F11>", function() require("dap").step_into() end, desc = "Debug: Step Into" },
      { "<F12>", function() require("dap").step_out() end, desc = "Debug: Step Out" },
      { "<leader>db", function() require("dap").toggle_breakpoint() end, desc = "Debug: Toggle Breakpoint" },
    },
    config = function()
      local dap = require("dap")

      -- ===== C/C++ (via lldb/codelldb) =====
      dap.adapters.lldb = {
        type = "executable",
        command = "lldb-vscode", -- Or "codelldb" if using VS Code's debugger
        name = "lldb",
      }

      dap.configurations.cpp = {
        {
          name = "Launch C++",
          type = "lldb",
          request = "launch",
          program = function()
            return vim.fn.input("Path to executable: ", vim.fn.getcwd() .. "/", "file")
          end,
          cwd = "${workspaceFolder}",
          args = {},
        },
      }
      dap.configurations.c = dap.configurations.cpp

      -- ===== Lua =====
      dap.adapters.lua = {
        type = "executable",
        command = "node",
        args = {
          os.getenv("HOME") .. "/.local/share/nvim/dapinstall/lua-debugger/extension/dist/debugger.js",
        },
      }

      dap.configurations.lua = {
        {
          type = "lua",
          request = "launch",
          name = "Launch Lua File",
          program = "${file}",
          cwd = "${workspaceFolder}",
        },
      }
    end,
  },

  -- Debugger UI
  {
    "rcarriga/nvim-dap-ui",
    dependencies = { "mfussenegger/nvim-dap", "nvim-neotest/nvim-nio" },
    config = function()
      require("dapui").setup({
        layouts = {
          {
            elements = {
              "scopes",
              "breakpoints",
              "stacks",
              "watches",
            },
            size = 40,
            position = "left",
          },
          {
            elements = {
              "repl",
              "console",
            },
            size = 10,
            position = "bottom",
          },
        },
      })

      -- Auto-open UI
      local dap, dapui = require("dap"), require("dapui")
      dap.listeners.after.event_initialized["dapui_config"] = function()
        dapui.open()
      end
      dap.listeners.before.event_terminated["dapui_config"] = function()
        dapui.close()
      end
      dap.listeners.before.event_exited["dapui_config"] = function()
        dapui.close()
      end
    end,
  },

  -- Debugger virtual text
  {
    "theHamsta/nvim-dap-virtual-text",
    dependencies = { "mfussenegger/nvim-dap" },
    config = true,
  },
}
