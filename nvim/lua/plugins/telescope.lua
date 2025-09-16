return {
  'nvim-telescope/telescope.nvim',
  tag = '0.1.8',
  dependencies = {
    'nvim-lua/plenary.nvim',
    -- Optional: For better sorting and performance
    { 'nvim-telescope/telescope-fzf-native.nvim', build = 'make' },
  },
  config = function()
    local telescope = require('telescope')
    local builtin = require('telescope.builtin')
    local actions = require('telescope.actions')

    -- Configure Telescope
    telescope.setup({
      defaults = {
        mappings = {
          i = {
            -- Close on first <Esc> press
            ['<Esc>'] = actions.close,
            -- Navigate results with <C-j/k>
            ['<C-j>'] = actions.move_selection_next,
            ['<C-k>'] = actions.move_selection_previous,
            -- Open in vertical/horizontal split
            ['<C-v>'] = actions.select_vertical,
            ['<C-x>'] = actions.select_horizontal,
          },
        },
        -- Default configuration (can be overridden in specific pickers)
        layout_strategy = 'horizontal',
        layout_config = {
          prompt_position = 'top',
        },
        sorting_strategy = 'ascending',
      },
      pickers = {
        find_files = {
          hidden = true, -- show hidden files
          no_ignore = false, -- respect .gitignore
        },
      },
    })

    -- Load extensions (fzf-native for better performance)
    telescope.load_extension('fzf')

    -- Keybindings
    vim.keymap.set('n', '<leader>ff', builtin.find_files, { desc = 'Find Files' })
    vim.keymap.set('n', '<leader>fg', function()
      builtin.live_grep({ additional_args = { '--hidden', '--glob', '!.git/*' } })
    end, { desc = 'Live Grep (all files)' })
    vim.keymap.set('n', '<leader>fb', builtin.buffers, { desc = 'Find Buffers' })
    vim.keymap.set('n', '<leader>fh', builtin.help_tags, { desc = 'Help Tags' })
    vim.keymap.set('n', '<leader>fr', builtin.oldfiles, { desc = 'Recent Files' })
    vim.keymap.set('n', '<leader>fc', builtin.commands, { desc = 'Commands' })
    vim.keymap.set('n', '<leader>fk', builtin.keymaps, { desc = 'Keymaps' })
    vim.keymap.set('n', '<leader>fd', builtin.diagnostics, { desc = 'Diagnostics' })
    vim.keymap.set('n', '<leader>fs', builtin.lsp_document_symbols, { desc = 'Document Symbols' })
    vim.keymap.set('n', '<leader>fw', builtin.grep_string, { desc = 'Search Current Word' })
    vim.keymap.set('n', '<leader>fz', builtin.current_buffer_fuzzy_find, { desc = 'Search in Current Buffer' })

    -- Git integration (requires fugitive plugin)
    vim.keymap.set('n', '<leader>gb', builtin.git_branches, { desc = 'Git Branches' })
    vim.keymap.set('n', '<leader>gc', builtin.git_commits, { desc = 'Git Commits' })
    vim.keymap.set('n', '<leader>gs', builtin.git_status, { desc = 'Git Status' })
  end,
}
