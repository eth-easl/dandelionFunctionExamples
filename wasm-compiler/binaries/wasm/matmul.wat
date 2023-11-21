(module
  (type (;0;) (func))
  (type (;1;) (func (result i32)))
  (type (;2;) (func (param i32 i32) (result i32)))
  (type (;3;) (func (param i32)))
  (type (;4;) (func (param i32) (result i32)))
  (type (;5;) (func (param i32 i32)))
  (func $__wasm_call_ctors (type 0))
  (func $_start (type 0)
    call $__dandelion_entry
    return
    unreachable)
  (func $__dandelion_entry (type 0)
    call $dandelion_init
    call $mat_mul
    call $dandelion_exit)
  (func $mat_mul (type 1) (result i32)
    (local i32 i32 i32 i32 i32 i64 i64 i64 i32 i32 i32 i32 i32 i32 i32 i64 i64 i64 i32 i32 i32 i32 i32 i64 i32 i64)
    global.get $__stack_pointer
    i32.const 48
    i32.sub
    local.tee 0
    global.set $__stack_pointer
    i32.const -1
    local.set 1
    block  ;; label = @1
      call $dandelion_input_set_count
      i32.const 1
      i32.ne
      br_if 0 (;@1;)
      i32.const -2
      local.set 1
      i32.const 0
      call $dandelion_input_buffer_count
      i32.const 1
      i32.ne
      br_if 0 (;@1;)
      i32.const -3
      local.set 1
      i32.const 0
      i32.const 0
      call $dandelion_get_input
      local.tee 2
      i32.load offset=12
      local.tee 3
      i32.const 8
      i32.lt_u
      br_if 0 (;@1;)
      i32.const -4
      local.set 1
      local.get 3
      i32.const -8
      i32.add
      local.tee 4
      i32.const 8
      i32.lt_u
      br_if 0 (;@1;)
      local.get 3
      i32.const 7
      i32.and
      br_if 0 (;@1;)
      local.get 4
      i32.const 3
      i32.shr_u
      i64.extend_i32_u
      local.tee 5
      local.get 5
      local.get 2
      i32.load offset=8
      local.tee 3
      i64.load
      local.tee 6
      i64.div_s
      local.tee 7
      local.get 6
      i64.mul
      i64.sub
      i64.const 0
      i64.ne
      br_if 0 (;@1;)
      local.get 6
      local.get 6
      i64.mul
      i32.wrap_i64
      i32.const 3
      i32.shl
      i32.const 8
      i32.add
      local.tee 8
      i32.const 8
      call $dandelion_alloc
      local.tee 9
      local.get 6
      i64.store
      block  ;; label = @2
        local.get 6
        i64.const 1
        i64.lt_s
        br_if 0 (;@2;)
        local.get 7
        i64.const 1
        i64.lt_s
        br_if 0 (;@2;)
        local.get 3
        i32.const 8
        i32.add
        local.set 10
        local.get 9
        i32.const 8
        i32.add
        local.set 11
        local.get 6
        i32.wrap_i64
        local.set 12
        local.get 7
        i32.wrap_i64
        local.tee 13
        i32.const 3
        i32.shl
        local.set 14
        local.get 7
        i64.const -2
        i64.and
        local.set 15
        local.get 7
        i64.const 1
        i64.and
        local.set 16
        i64.const 0
        local.set 17
        local.get 3
        i32.const 16
        i32.add
        local.tee 18
        local.set 19
        loop  ;; label = @3
          local.get 17
          i32.wrap_i64
          local.tee 1
          local.get 12
          i32.mul
          local.set 20
          local.get 1
          local.get 13
          i32.mul
          local.set 21
          local.get 18
          local.set 22
          i64.const 0
          local.set 23
          loop  ;; label = @4
            local.get 11
            local.get 20
            local.get 23
            i32.wrap_i64
            local.tee 24
            i32.add
            i32.const 3
            i32.shl
            i32.add
            local.tee 4
            i64.load
            local.set 25
            i64.const 0
            local.set 5
            block  ;; label = @5
              local.get 7
              i64.const 1
              i64.eq
              br_if 0 (;@5;)
              i64.const 0
              local.set 5
              local.get 22
              local.set 1
              local.get 19
              local.set 3
              loop  ;; label = @6
                local.get 4
                local.get 25
                local.get 1
                i32.const -8
                i32.add
                i64.load
                local.get 3
                i32.const -8
                i32.add
                i64.load
                i64.mul
                i64.add
                local.tee 25
                i64.store
                local.get 4
                local.get 25
                local.get 1
                i64.load
                local.get 3
                i64.load
                i64.mul
                i64.add
                local.tee 25
                i64.store
                local.get 1
                i32.const 16
                i32.add
                local.set 1
                local.get 3
                i32.const 16
                i32.add
                local.set 3
                local.get 15
                local.get 5
                i64.const 2
                i64.add
                local.tee 5
                i64.ne
                br_if 0 (;@6;)
              end
            end
            block  ;; label = @5
              local.get 16
              i64.eqz
              br_if 0 (;@5;)
              local.get 4
              local.get 25
              local.get 10
              local.get 24
              local.get 13
              i32.mul
              local.get 5
              i32.wrap_i64
              local.tee 1
              i32.add
              i32.const 3
              i32.shl
              i32.add
              i64.load
              local.get 10
              local.get 21
              local.get 1
              i32.add
              i32.const 3
              i32.shl
              i32.add
              i64.load
              i64.mul
              i64.add
              i64.store
            end
            local.get 22
            local.get 14
            i32.add
            local.set 22
            local.get 23
            i64.const 1
            i64.add
            local.tee 23
            local.get 6
            i64.ne
            br_if 0 (;@4;)
          end
          local.get 19
          local.get 14
          i32.add
          local.set 19
          local.get 17
          i64.const 1
          i64.add
          local.tee 17
          local.get 6
          i64.ne
          br_if 0 (;@3;)
        end
      end
      local.get 0
      i32.const 24
      i32.add
      i32.const 8
      i32.add
      local.tee 1
      local.get 9
      i32.store
      local.get 0
      i32.const 24
      i32.add
      i32.const 16
      i32.add
      local.get 2
      i32.load offset=16
      local.tee 3
      i32.store
      local.get 0
      i32.const 16
      i32.add
      local.get 3
      i32.store
      local.get 0
      local.get 8
      i32.store offset=36
      local.get 0
      i32.const 8
      i32.add
      local.get 1
      i64.load
      i64.store
      local.get 0
      i64.const 0
      i64.store offset=24
      local.get 0
      i64.const 0
      i64.store
      i32.const 0
      local.set 1
      i32.const 0
      local.get 0
      call $dandelion_add_output
    end
    local.get 0
    i32.const 48
    i32.add
    global.set $__stack_pointer
    local.get 1)
  (func $dandelion_init (type 0)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)
    global.get $__stack_pointer
    local.set 0
    i32.const 32
    local.set 1
    local.get 0
    local.get 1
    i32.sub
    local.set 2
    local.get 2
    global.set $__stack_pointer
    call $__dandelion_system_init
    i32.const 0
    local.set 3
    local.get 3
    i32.load offset=1048
    local.set 4
    i32.const 20
    local.set 5
    local.get 4
    local.get 5
    i32.mul
    local.set 6
    i32.const 4
    local.set 7
    local.get 6
    local.get 7
    call $dandelion_alloc
    local.set 8
    i32.const 0
    local.set 9
    local.get 9
    local.get 8
    i32.store offset=1024
    i32.const 0
    local.set 10
    local.get 2
    local.get 10
    i32.store offset=28
    block  ;; label = @1
      loop  ;; label = @2
        local.get 2
        i32.load offset=28
        local.set 11
        i32.const 0
        local.set 12
        local.get 12
        i32.load offset=1048
        local.set 13
        local.get 11
        local.set 14
        local.get 13
        local.set 15
        local.get 14
        local.get 15
        i32.lt_u
        local.set 16
        i32.const 1
        local.set 17
        local.get 16
        local.get 17
        i32.and
        local.set 18
        local.get 18
        i32.eqz
        br_if 1 (;@1;)
        i32.const 0
        local.set 19
        local.get 19
        i32.load offset=1024
        local.set 20
        local.get 2
        i32.load offset=28
        local.set 21
        i32.const 20
        local.set 22
        local.get 21
        local.get 22
        i32.mul
        local.set 23
        local.get 20
        local.get 23
        i32.add
        local.set 24
        local.get 2
        local.get 24
        i32.store offset=24
        i32.const 0
        local.set 25
        local.get 25
        i32.load offset=1052
        local.set 26
        local.get 2
        i32.load offset=28
        local.set 27
        i32.const 1
        local.set 28
        local.get 27
        local.get 28
        i32.add
        local.set 29
        i32.const 12
        local.set 30
        local.get 29
        local.get 30
        i32.mul
        local.set 31
        local.get 26
        local.get 31
        i32.add
        local.set 32
        local.get 32
        i32.load offset=8
        local.set 33
        i32.const 0
        local.set 34
        local.get 34
        i32.load offset=1052
        local.set 35
        local.get 2
        i32.load offset=28
        local.set 36
        i32.const 12
        local.set 37
        local.get 36
        local.get 37
        i32.mul
        local.set 38
        local.get 35
        local.get 38
        i32.add
        local.set 39
        local.get 39
        i32.load offset=8
        local.set 40
        local.get 33
        local.get 40
        i32.sub
        local.set 41
        local.get 2
        local.get 41
        i32.store offset=20
        i32.const 0
        local.set 42
        local.get 42
        i32.load offset=1052
        local.set 43
        local.get 2
        i32.load offset=28
        local.set 44
        i32.const 12
        local.set 45
        local.get 44
        local.get 45
        i32.mul
        local.set 46
        local.get 43
        local.get 46
        i32.add
        local.set 47
        local.get 47
        i32.load
        local.set 48
        local.get 2
        i32.load offset=24
        local.set 49
        local.get 49
        local.get 48
        i32.store
        i32.const 0
        local.set 50
        local.get 50
        i32.load offset=1052
        local.set 51
        local.get 2
        i32.load offset=28
        local.set 52
        i32.const 12
        local.set 53
        local.get 52
        local.get 53
        i32.mul
        local.set 54
        local.get 51
        local.get 54
        i32.add
        local.set 55
        local.get 55
        i32.load offset=4
        local.set 56
        local.get 2
        i32.load offset=24
        local.set 57
        local.get 57
        local.get 56
        i32.store offset=4
        local.get 2
        i32.load offset=20
        local.set 58
        i32.const 20
        local.set 59
        local.get 58
        local.get 59
        i32.mul
        local.set 60
        i32.const 4
        local.set 61
        local.get 60
        local.get 61
        call $dandelion_alloc
        local.set 62
        local.get 2
        i32.load offset=24
        local.set 63
        local.get 63
        local.get 62
        i32.store offset=8
        local.get 2
        i32.load offset=20
        local.set 64
        local.get 2
        i32.load offset=24
        local.set 65
        local.get 65
        local.get 64
        i32.store offset=12
        local.get 2
        i32.load offset=20
        local.set 66
        local.get 2
        i32.load offset=24
        local.set 67
        local.get 67
        local.get 66
        i32.store offset=16
        i32.const 0
        local.set 68
        local.get 2
        local.get 68
        i32.store offset=16
        block  ;; label = @3
          loop  ;; label = @4
            local.get 2
            i32.load offset=16
            local.set 69
            local.get 2
            i32.load offset=20
            local.set 70
            local.get 69
            local.set 71
            local.get 70
            local.set 72
            local.get 71
            local.get 72
            i32.lt_u
            local.set 73
            i32.const 1
            local.set 74
            local.get 73
            local.get 74
            i32.and
            local.set 75
            local.get 75
            i32.eqz
            br_if 1 (;@3;)
            i32.const 0
            local.set 76
            local.get 76
            i32.load offset=1052
            local.set 77
            local.get 2
            i32.load offset=28
            local.set 78
            i32.const 12
            local.set 79
            local.get 78
            local.get 79
            i32.mul
            local.set 80
            local.get 77
            local.get 80
            i32.add
            local.set 81
            local.get 81
            i32.load offset=8
            local.set 82
            local.get 2
            i32.load offset=16
            local.set 83
            local.get 82
            local.get 83
            i32.add
            local.set 84
            local.get 2
            local.get 84
            i32.store offset=12
            i32.const 0
            local.set 85
            local.get 85
            i32.load offset=1064
            local.set 86
            local.get 2
            i32.load offset=12
            local.set 87
            i32.const 20
            local.set 88
            local.get 87
            local.get 88
            i32.mul
            local.set 89
            local.get 86
            local.get 89
            i32.add
            local.set 90
            local.get 90
            i32.load
            local.set 91
            local.get 2
            i32.load offset=24
            local.set 92
            local.get 92
            i32.load offset=8
            local.set 93
            local.get 2
            i32.load offset=16
            local.set 94
            i32.const 20
            local.set 95
            local.get 94
            local.get 95
            i32.mul
            local.set 96
            local.get 93
            local.get 96
            i32.add
            local.set 97
            local.get 97
            local.get 91
            i32.store
            i32.const 0
            local.set 98
            local.get 98
            i32.load offset=1064
            local.set 99
            local.get 2
            i32.load offset=12
            local.set 100
            i32.const 20
            local.set 101
            local.get 100
            local.get 101
            i32.mul
            local.set 102
            local.get 99
            local.get 102
            i32.add
            local.set 103
            local.get 103
            i32.load offset=4
            local.set 104
            local.get 2
            i32.load offset=24
            local.set 105
            local.get 105
            i32.load offset=8
            local.set 106
            local.get 2
            i32.load offset=16
            local.set 107
            i32.const 20
            local.set 108
            local.get 107
            local.get 108
            i32.mul
            local.set 109
            local.get 106
            local.get 109
            i32.add
            local.set 110
            local.get 110
            local.get 104
            i32.store offset=4
            i32.const 0
            local.set 111
            local.get 111
            i32.load offset=1064
            local.set 112
            local.get 2
            i32.load offset=12
            local.set 113
            i32.const 20
            local.set 114
            local.get 113
            local.get 114
            i32.mul
            local.set 115
            local.get 112
            local.get 115
            i32.add
            local.set 116
            local.get 116
            i32.load offset=8
            local.set 117
            local.get 2
            i32.load offset=24
            local.set 118
            local.get 118
            i32.load offset=8
            local.set 119
            local.get 2
            i32.load offset=16
            local.set 120
            i32.const 20
            local.set 121
            local.get 120
            local.get 121
            i32.mul
            local.set 122
            local.get 119
            local.get 122
            i32.add
            local.set 123
            local.get 123
            local.get 117
            i32.store offset=8
            i32.const 0
            local.set 124
            local.get 124
            i32.load offset=1064
            local.set 125
            local.get 2
            i32.load offset=12
            local.set 126
            i32.const 20
            local.set 127
            local.get 126
            local.get 127
            i32.mul
            local.set 128
            local.get 125
            local.get 128
            i32.add
            local.set 129
            local.get 129
            i32.load offset=12
            local.set 130
            local.get 2
            i32.load offset=24
            local.set 131
            local.get 131
            i32.load offset=8
            local.set 132
            local.get 2
            i32.load offset=16
            local.set 133
            i32.const 20
            local.set 134
            local.get 133
            local.get 134
            i32.mul
            local.set 135
            local.get 132
            local.get 135
            i32.add
            local.set 136
            local.get 136
            local.get 130
            i32.store offset=12
            local.get 2
            i32.load offset=16
            local.set 137
            i32.const 1
            local.set 138
            local.get 137
            local.get 138
            i32.add
            local.set 139
            local.get 2
            local.get 139
            i32.store offset=16
            br 0 (;@4;)
          end
        end
        local.get 2
        i32.load offset=28
        local.set 140
        i32.const 1
        local.set 141
        local.get 140
        local.get 141
        i32.add
        local.set 142
        local.get 2
        local.get 142
        i32.store offset=28
        br 0 (;@2;)
      end
    end
    i32.const 0
    local.set 143
    local.get 143
    i32.load offset=1056
    local.set 144
    i32.const 20
    local.set 145
    local.get 144
    local.get 145
    i32.mul
    local.set 146
    i32.const 4
    local.set 147
    local.get 146
    local.get 147
    call $dandelion_alloc
    local.set 148
    i32.const 0
    local.set 149
    local.get 149
    local.get 148
    i32.store offset=1028
    i32.const 0
    local.set 150
    local.get 2
    local.get 150
    i32.store offset=8
    block  ;; label = @1
      loop  ;; label = @2
        local.get 2
        i32.load offset=8
        local.set 151
        i32.const 0
        local.set 152
        local.get 152
        i32.load offset=1056
        local.set 153
        local.get 151
        local.set 154
        local.get 153
        local.set 155
        local.get 154
        local.get 155
        i32.lt_u
        local.set 156
        i32.const 1
        local.set 157
        local.get 156
        local.get 157
        i32.and
        local.set 158
        local.get 158
        i32.eqz
        br_if 1 (;@1;)
        i32.const 0
        local.set 159
        local.get 159
        i32.load offset=1060
        local.set 160
        local.get 2
        i32.load offset=8
        local.set 161
        i32.const 12
        local.set 162
        local.get 161
        local.get 162
        i32.mul
        local.set 163
        local.get 160
        local.get 163
        i32.add
        local.set 164
        local.get 164
        i32.load
        local.set 165
        i32.const 0
        local.set 166
        local.get 166
        i32.load offset=1028
        local.set 167
        local.get 2
        i32.load offset=8
        local.set 168
        i32.const 20
        local.set 169
        local.get 168
        local.get 169
        i32.mul
        local.set 170
        local.get 167
        local.get 170
        i32.add
        local.set 171
        local.get 171
        local.get 165
        i32.store
        i32.const 0
        local.set 172
        local.get 172
        i32.load offset=1060
        local.set 173
        local.get 2
        i32.load offset=8
        local.set 174
        i32.const 12
        local.set 175
        local.get 174
        local.get 175
        i32.mul
        local.set 176
        local.get 173
        local.get 176
        i32.add
        local.set 177
        local.get 177
        i32.load offset=4
        local.set 178
        i32.const 0
        local.set 179
        local.get 179
        i32.load offset=1028
        local.set 180
        local.get 2
        i32.load offset=8
        local.set 181
        i32.const 20
        local.set 182
        local.get 181
        local.get 182
        i32.mul
        local.set 183
        local.get 180
        local.get 183
        i32.add
        local.set 184
        local.get 184
        local.get 178
        i32.store offset=4
        i32.const 0
        local.set 185
        local.get 185
        i32.load offset=1028
        local.set 186
        local.get 2
        i32.load offset=8
        local.set 187
        i32.const 20
        local.set 188
        local.get 187
        local.get 188
        i32.mul
        local.set 189
        local.get 186
        local.get 189
        i32.add
        local.set 190
        i32.const 0
        local.set 191
        local.get 190
        local.get 191
        i32.store offset=8
        i32.const 0
        local.set 192
        local.get 192
        i32.load offset=1028
        local.set 193
        local.get 2
        i32.load offset=8
        local.set 194
        i32.const 20
        local.set 195
        local.get 194
        local.get 195
        i32.mul
        local.set 196
        local.get 193
        local.get 196
        i32.add
        local.set 197
        i32.const 0
        local.set 198
        local.get 197
        local.get 198
        i32.store offset=12
        i32.const 0
        local.set 199
        local.get 199
        i32.load offset=1028
        local.set 200
        local.get 2
        i32.load offset=8
        local.set 201
        i32.const 20
        local.set 202
        local.get 201
        local.get 202
        i32.mul
        local.set 203
        local.get 200
        local.get 203
        i32.add
        local.set 204
        i32.const 0
        local.set 205
        local.get 204
        local.get 205
        i32.store offset=16
        local.get 2
        i32.load offset=8
        local.set 206
        i32.const 1
        local.set 207
        local.get 206
        local.get 207
        i32.add
        local.set 208
        local.get 2
        local.get 208
        i32.store offset=8
        br 0 (;@2;)
      end
    end
    i32.const 32
    local.set 209
    local.get 2
    local.get 209
    i32.add
    local.set 210
    local.get 210
    global.set $__stack_pointer
    return)
  (func $dandelion_alloc (type 2) (param i32 i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)
    global.get $__stack_pointer
    local.set 2
    i32.const 16
    local.set 3
    local.get 2
    local.get 3
    i32.sub
    local.set 4
    local.get 4
    local.get 0
    i32.store offset=8
    local.get 4
    local.get 1
    i32.store offset=4
    i32.const 0
    local.set 5
    local.get 5
    i32.load offset=1032
    local.set 6
    block  ;; label = @1
      local.get 6
      br_if 0 (;@1;)
      i32.const 0
      local.set 7
      local.get 7
      i32.load offset=1040
      local.set 8
      i32.const 0
      local.set 9
      local.get 9
      local.get 8
      i32.store offset=1032
    end
    i32.const 0
    local.set 10
    local.get 10
    i32.load offset=1032
    local.set 11
    i32.const 1
    local.set 12
    local.get 11
    local.get 12
    i32.sub
    local.set 13
    local.get 4
    i32.load offset=4
    local.set 14
    i32.const 1
    local.set 15
    local.get 14
    local.get 15
    i32.sub
    local.set 16
    local.get 13
    local.get 16
    i32.or
    local.set 17
    i32.const 1
    local.set 18
    local.get 17
    local.get 18
    i32.add
    local.set 19
    i32.const 0
    local.set 20
    local.get 20
    local.get 19
    i32.store offset=1032
    i32.const 0
    local.set 21
    local.get 21
    i32.load offset=1032
    local.set 22
    local.get 4
    i32.load offset=8
    local.set 23
    local.get 22
    local.get 23
    i32.add
    local.set 24
    i32.const 0
    local.set 25
    local.get 25
    i32.load offset=1044
    local.set 26
    local.get 24
    local.set 27
    local.get 26
    local.set 28
    local.get 27
    local.get 28
    i32.gt_u
    local.set 29
    i32.const 1
    local.set 30
    local.get 29
    local.get 30
    i32.and
    local.set 31
    block  ;; label = @1
      block  ;; label = @2
        local.get 31
        i32.eqz
        br_if 0 (;@2;)
        i32.const 0
        local.set 32
        local.get 4
        local.get 32
        i32.store offset=12
        br 1 (;@1;)
      end
      i32.const 0
      local.set 33
      local.get 33
      i32.load offset=1032
      local.set 34
      local.get 4
      local.get 34
      i32.store
      local.get 4
      i32.load offset=8
      local.set 35
      i32.const 0
      local.set 36
      local.get 36
      i32.load offset=1032
      local.set 37
      local.get 37
      local.get 35
      i32.add
      local.set 38
      i32.const 0
      local.set 39
      local.get 39
      local.get 38
      i32.store offset=1032
      local.get 4
      i32.load
      local.set 40
      local.get 4
      local.get 40
      i32.store offset=12
    end
    local.get 4
    i32.load offset=12
    local.set 41
    local.get 41
    return)
  (func $dandelion_exit (type 3) (param i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)
    global.get $__stack_pointer
    local.set 1
    i32.const 48
    local.set 2
    local.get 1
    local.get 2
    i32.sub
    local.set 3
    local.get 3
    global.set $__stack_pointer
    local.get 3
    local.get 0
    i32.store offset=44
    local.get 3
    i32.load offset=44
    local.set 4
    i32.const 0
    local.set 5
    local.get 5
    local.get 4
    i32.store offset=1036
    i32.const 0
    local.set 6
    local.get 3
    local.get 6
    i32.store offset=40
    i32.const 0
    local.set 7
    local.get 3
    local.get 7
    i32.store offset=36
    block  ;; label = @1
      loop  ;; label = @2
        local.get 3
        i32.load offset=36
        local.set 8
        i32.const 0
        local.set 9
        local.get 9
        i32.load offset=1056
        local.set 10
        local.get 8
        local.set 11
        local.get 10
        local.set 12
        local.get 11
        local.get 12
        i32.lt_u
        local.set 13
        i32.const 1
        local.set 14
        local.get 13
        local.get 14
        i32.and
        local.set 15
        local.get 15
        i32.eqz
        br_if 1 (;@1;)
        i32.const 0
        local.set 16
        local.get 16
        i32.load offset=1028
        local.set 17
        local.get 3
        i32.load offset=36
        local.set 18
        i32.const 20
        local.set 19
        local.get 18
        local.get 19
        i32.mul
        local.set 20
        local.get 17
        local.get 20
        i32.add
        local.set 21
        local.get 21
        i32.load offset=12
        local.set 22
        local.get 3
        i32.load offset=40
        local.set 23
        local.get 23
        local.get 22
        i32.add
        local.set 24
        local.get 3
        local.get 24
        i32.store offset=40
        local.get 3
        i32.load offset=36
        local.set 25
        i32.const 1
        local.set 26
        local.get 25
        local.get 26
        i32.add
        local.set 27
        local.get 3
        local.get 27
        i32.store offset=36
        br 0 (;@2;)
      end
    end
    local.get 3
    i32.load offset=40
    local.set 28
    i32.const 20
    local.set 29
    local.get 28
    local.get 29
    i32.mul
    local.set 30
    i32.const 4
    local.set 31
    local.get 30
    local.get 31
    call $dandelion_alloc
    local.set 32
    i32.const 0
    local.set 33
    local.get 33
    local.get 32
    i32.store offset=1068
    i32.const 0
    local.set 34
    local.get 3
    local.get 34
    i32.store offset=32
    i32.const 0
    local.set 35
    local.get 3
    local.get 35
    i32.store offset=28
    block  ;; label = @1
      loop  ;; label = @2
        local.get 3
        i32.load offset=28
        local.set 36
        i32.const 0
        local.set 37
        local.get 37
        i32.load offset=1056
        local.set 38
        local.get 36
        local.set 39
        local.get 38
        local.set 40
        local.get 39
        local.get 40
        i32.lt_u
        local.set 41
        i32.const 1
        local.set 42
        local.get 41
        local.get 42
        i32.and
        local.set 43
        local.get 43
        i32.eqz
        br_if 1 (;@1;)
        local.get 3
        i32.load offset=32
        local.set 44
        i32.const 0
        local.set 45
        local.get 45
        i32.load offset=1060
        local.set 46
        local.get 3
        i32.load offset=28
        local.set 47
        i32.const 12
        local.set 48
        local.get 47
        local.get 48
        i32.mul
        local.set 49
        local.get 46
        local.get 49
        i32.add
        local.set 50
        local.get 50
        local.get 44
        i32.store offset=8
        i32.const 0
        local.set 51
        local.get 51
        i32.load offset=1028
        local.set 52
        local.get 3
        i32.load offset=28
        local.set 53
        i32.const 20
        local.set 54
        local.get 53
        local.get 54
        i32.mul
        local.set 55
        local.get 52
        local.get 55
        i32.add
        local.set 56
        local.get 3
        local.get 56
        i32.store offset=24
        local.get 3
        i32.load offset=24
        local.set 57
        local.get 57
        i32.load offset=12
        local.set 58
        local.get 3
        i32.load offset=32
        local.set 59
        local.get 59
        local.get 58
        i32.add
        local.set 60
        local.get 3
        local.get 60
        i32.store offset=32
        i32.const 0
        local.set 61
        local.get 3
        local.get 61
        i32.store offset=20
        block  ;; label = @3
          loop  ;; label = @4
            local.get 3
            i32.load offset=20
            local.set 62
            local.get 3
            i32.load offset=24
            local.set 63
            local.get 63
            i32.load offset=12
            local.set 64
            local.get 62
            local.set 65
            local.get 64
            local.set 66
            local.get 65
            local.get 66
            i32.lt_u
            local.set 67
            i32.const 1
            local.set 68
            local.get 67
            local.get 68
            i32.and
            local.set 69
            local.get 69
            i32.eqz
            br_if 1 (;@3;)
            local.get 3
            i32.load offset=24
            local.set 70
            local.get 70
            i32.load offset=8
            local.set 71
            local.get 3
            i32.load offset=20
            local.set 72
            i32.const 20
            local.set 73
            local.get 72
            local.get 73
            i32.mul
            local.set 74
            local.get 71
            local.get 74
            i32.add
            local.set 75
            local.get 3
            local.get 75
            i32.store offset=16
            i32.const 0
            local.set 76
            local.get 76
            i32.load offset=1068
            local.set 77
            i32.const 0
            local.set 78
            local.get 78
            i32.load offset=1060
            local.set 79
            local.get 3
            i32.load offset=28
            local.set 80
            i32.const 12
            local.set 81
            local.get 80
            local.get 81
            i32.mul
            local.set 82
            local.get 79
            local.get 82
            i32.add
            local.set 83
            local.get 83
            i32.load offset=8
            local.set 84
            local.get 3
            i32.load offset=20
            local.set 85
            local.get 84
            local.get 85
            i32.add
            local.set 86
            i32.const 20
            local.set 87
            local.get 86
            local.get 87
            i32.mul
            local.set 88
            local.get 77
            local.get 88
            i32.add
            local.set 89
            local.get 3
            local.get 89
            i32.store offset=12
            local.get 3
            i32.load offset=16
            local.set 90
            local.get 90
            i32.load
            local.set 91
            local.get 3
            i32.load offset=12
            local.set 92
            local.get 92
            local.get 91
            i32.store
            local.get 3
            i32.load offset=16
            local.set 93
            local.get 93
            i32.load offset=4
            local.set 94
            local.get 3
            i32.load offset=12
            local.set 95
            local.get 95
            local.get 94
            i32.store offset=4
            local.get 3
            i32.load offset=16
            local.set 96
            local.get 96
            i32.load offset=8
            local.set 97
            local.get 3
            i32.load offset=12
            local.set 98
            local.get 98
            local.get 97
            i32.store offset=8
            local.get 3
            i32.load offset=16
            local.set 99
            local.get 99
            i32.load offset=12
            local.set 100
            local.get 3
            i32.load offset=12
            local.set 101
            local.get 101
            local.get 100
            i32.store offset=12
            local.get 3
            i32.load offset=20
            local.set 102
            i32.const 1
            local.set 103
            local.get 102
            local.get 103
            i32.add
            local.set 104
            local.get 3
            local.get 104
            i32.store offset=20
            br 0 (;@4;)
          end
        end
        local.get 3
        i32.load offset=28
        local.set 105
        i32.const 1
        local.set 106
        local.get 105
        local.get 106
        i32.add
        local.set 107
        local.get 3
        local.get 107
        i32.store offset=28
        br 0 (;@2;)
      end
    end
    local.get 3
    i32.load offset=32
    local.set 108
    i32.const 0
    local.set 109
    local.get 109
    i32.load offset=1060
    local.set 110
    i32.const 0
    local.set 111
    local.get 111
    i32.load offset=1056
    local.set 112
    i32.const 12
    local.set 113
    local.get 112
    local.get 113
    i32.mul
    local.set 114
    local.get 110
    local.get 114
    i32.add
    local.set 115
    local.get 115
    local.get 108
    i32.store offset=8
    call $__dandelion_system_exit
    i32.const 48
    local.set 116
    local.get 3
    local.get 116
    i32.add
    local.set 117
    local.get 117
    global.set $__stack_pointer
    return)
  (func $dandelion_set_thread_pointer (type 3) (param i32)
    (local i32 i32 i32 i32 i32 i32)
    global.get $__stack_pointer
    local.set 1
    i32.const 16
    local.set 2
    local.get 1
    local.get 2
    i32.sub
    local.set 3
    local.get 3
    global.set $__stack_pointer
    local.get 3
    local.get 0
    i32.store offset=12
    local.get 3
    i32.load offset=12
    local.set 4
    local.get 4
    call $__dandelion_system_set_thread_pointer
    i32.const 16
    local.set 5
    local.get 3
    local.get 5
    i32.add
    local.set 6
    local.get 6
    global.set $__stack_pointer
    return)
  (func $dandelion_input_set_count (type 1) (result i32)
    (local i32 i32)
    i32.const 0
    local.set 0
    local.get 0
    i32.load offset=1048
    local.set 1
    local.get 1
    return)
  (func $dandelion_output_set_count (type 1) (result i32)
    (local i32 i32)
    i32.const 0
    local.set 0
    local.get 0
    i32.load offset=1056
    local.set 1
    local.get 1
    return)
  (func $dandelion_input_buffer_count (type 4) (param i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)
    global.get $__stack_pointer
    local.set 1
    i32.const 16
    local.set 2
    local.get 1
    local.get 2
    i32.sub
    local.set 3
    local.get 3
    local.get 0
    i32.store offset=8
    local.get 3
    i32.load offset=8
    local.set 4
    i32.const 0
    local.set 5
    local.get 5
    i32.load offset=1048
    local.set 6
    local.get 4
    local.set 7
    local.get 6
    local.set 8
    local.get 7
    local.get 8
    i32.ge_u
    local.set 9
    i32.const 1
    local.set 10
    local.get 9
    local.get 10
    i32.and
    local.set 11
    block  ;; label = @1
      block  ;; label = @2
        local.get 11
        i32.eqz
        br_if 0 (;@2;)
        i32.const 0
        local.set 12
        local.get 3
        local.get 12
        i32.store offset=12
        br 1 (;@1;)
      end
      i32.const 0
      local.set 13
      local.get 13
      i32.load offset=1024
      local.set 14
      local.get 3
      i32.load offset=8
      local.set 15
      i32.const 20
      local.set 16
      local.get 15
      local.get 16
      i32.mul
      local.set 17
      local.get 14
      local.get 17
      i32.add
      local.set 18
      local.get 18
      i32.load offset=12
      local.set 19
      local.get 3
      local.get 19
      i32.store offset=12
    end
    local.get 3
    i32.load offset=12
    local.set 20
    local.get 20
    return)
  (func $dandelion_get_input (type 2) (param i32 i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)
    global.get $__stack_pointer
    local.set 2
    i32.const 16
    local.set 3
    local.get 2
    local.get 3
    i32.sub
    local.set 4
    local.get 4
    local.get 0
    i32.store offset=8
    local.get 4
    local.get 1
    i32.store offset=4
    local.get 4
    i32.load offset=8
    local.set 5
    i32.const 0
    local.set 6
    local.get 6
    i32.load offset=1048
    local.set 7
    local.get 5
    local.set 8
    local.get 7
    local.set 9
    local.get 8
    local.get 9
    i32.ge_u
    local.set 10
    i32.const 1
    local.set 11
    local.get 10
    local.get 11
    i32.and
    local.set 12
    block  ;; label = @1
      block  ;; label = @2
        local.get 12
        i32.eqz
        br_if 0 (;@2;)
        i32.const 0
        local.set 13
        local.get 4
        local.get 13
        i32.store offset=12
        br 1 (;@1;)
      end
      local.get 4
      i32.load offset=4
      local.set 14
      i32.const 0
      local.set 15
      local.get 15
      i32.load offset=1024
      local.set 16
      local.get 4
      i32.load offset=8
      local.set 17
      i32.const 20
      local.set 18
      local.get 17
      local.get 18
      i32.mul
      local.set 19
      local.get 16
      local.get 19
      i32.add
      local.set 20
      local.get 20
      i32.load offset=12
      local.set 21
      local.get 14
      local.set 22
      local.get 21
      local.set 23
      local.get 22
      local.get 23
      i32.ge_u
      local.set 24
      i32.const 1
      local.set 25
      local.get 24
      local.get 25
      i32.and
      local.set 26
      block  ;; label = @2
        local.get 26
        i32.eqz
        br_if 0 (;@2;)
        i32.const 0
        local.set 27
        local.get 4
        local.get 27
        i32.store offset=12
        br 1 (;@1;)
      end
      i32.const 0
      local.set 28
      local.get 28
      i32.load offset=1024
      local.set 29
      local.get 4
      i32.load offset=8
      local.set 30
      i32.const 20
      local.set 31
      local.get 30
      local.get 31
      i32.mul
      local.set 32
      local.get 29
      local.get 32
      i32.add
      local.set 33
      local.get 33
      i32.load offset=8
      local.set 34
      local.get 4
      i32.load offset=4
      local.set 35
      i32.const 20
      local.set 36
      local.get 35
      local.get 36
      i32.mul
      local.set 37
      local.get 34
      local.get 37
      i32.add
      local.set 38
      local.get 4
      local.get 38
      i32.store offset=12
    end
    local.get 4
    i32.load offset=12
    local.set 39
    local.get 39
    return)
  (func $dandelion_add_output (type 5) (param i32 i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i64 i32 i32 i32 i32 i32 i32 i32 i64 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i64 i32 i32 i32 i32 i32 i32 i32 i64 i32 i32)
    global.get $__stack_pointer
    local.set 2
    i32.const 32
    local.set 3
    local.get 2
    local.get 3
    i32.sub
    local.set 4
    local.get 4
    global.set $__stack_pointer
    local.get 4
    local.get 0
    i32.store offset=28
    local.get 4
    i32.load offset=28
    local.set 5
    i32.const 0
    local.set 6
    local.get 6
    i32.load offset=1056
    local.set 7
    local.get 5
    local.set 8
    local.get 7
    local.set 9
    local.get 8
    local.get 9
    i32.ge_u
    local.set 10
    i32.const 1
    local.set 11
    local.get 10
    local.get 11
    i32.and
    local.set 12
    block  ;; label = @1
      block  ;; label = @2
        local.get 12
        i32.eqz
        br_if 0 (;@2;)
        br 1 (;@1;)
      end
      i32.const 0
      local.set 13
      local.get 13
      i32.load offset=1028
      local.set 14
      local.get 4
      i32.load offset=28
      local.set 15
      i32.const 20
      local.set 16
      local.get 15
      local.get 16
      i32.mul
      local.set 17
      local.get 14
      local.get 17
      i32.add
      local.set 18
      local.get 4
      local.get 18
      i32.store offset=24
      local.get 4
      i32.load offset=24
      local.set 19
      local.get 19
      i32.load offset=12
      local.set 20
      local.get 4
      i32.load offset=24
      local.set 21
      local.get 21
      i32.load offset=16
      local.set 22
      local.get 20
      local.set 23
      local.get 22
      local.set 24
      local.get 23
      local.get 24
      i32.eq
      local.set 25
      i32.const 1
      local.set 26
      local.get 25
      local.get 26
      i32.and
      local.set 27
      block  ;; label = @2
        local.get 27
        i32.eqz
        br_if 0 (;@2;)
        local.get 4
        i32.load offset=24
        local.set 28
        local.get 28
        i32.load offset=16
        local.set 29
        i32.const 1
        local.set 30
        local.get 29
        local.get 30
        i32.shl
        local.set 31
        local.get 4
        local.get 31
        i32.store offset=20
        local.get 4
        i32.load offset=20
        local.set 32
        block  ;; label = @3
          local.get 32
          br_if 0 (;@3;)
          i32.const 1
          local.set 33
          local.get 4
          local.get 33
          i32.store offset=20
        end
        local.get 4
        i32.load offset=20
        local.set 34
        i32.const 20
        local.set 35
        local.get 34
        local.get 35
        i32.mul
        local.set 36
        i32.const 4
        local.set 37
        local.get 36
        local.get 37
        call $dandelion_alloc
        local.set 38
        local.get 4
        local.get 38
        i32.store offset=16
        i32.const 0
        local.set 39
        local.get 4
        local.get 39
        i32.store offset=12
        block  ;; label = @3
          loop  ;; label = @4
            local.get 4
            i32.load offset=12
            local.set 40
            local.get 4
            i32.load offset=24
            local.set 41
            local.get 41
            i32.load offset=12
            local.set 42
            local.get 40
            local.set 43
            local.get 42
            local.set 44
            local.get 43
            local.get 44
            i32.lt_u
            local.set 45
            i32.const 1
            local.set 46
            local.get 45
            local.get 46
            i32.and
            local.set 47
            local.get 47
            i32.eqz
            br_if 1 (;@3;)
            local.get 4
            i32.load offset=16
            local.set 48
            local.get 4
            i32.load offset=12
            local.set 49
            i32.const 20
            local.set 50
            local.get 49
            local.get 50
            i32.mul
            local.set 51
            local.get 48
            local.get 51
            i32.add
            local.set 52
            local.get 4
            i32.load offset=24
            local.set 53
            local.get 53
            i32.load offset=8
            local.set 54
            local.get 4
            i32.load offset=12
            local.set 55
            i32.const 20
            local.set 56
            local.get 55
            local.get 56
            i32.mul
            local.set 57
            local.get 54
            local.get 57
            i32.add
            local.set 58
            local.get 58
            i64.load align=4
            local.set 59
            local.get 52
            local.get 59
            i64.store align=4
            i32.const 16
            local.set 60
            local.get 52
            local.get 60
            i32.add
            local.set 61
            local.get 58
            local.get 60
            i32.add
            local.set 62
            local.get 62
            i32.load
            local.set 63
            local.get 61
            local.get 63
            i32.store
            i32.const 8
            local.set 64
            local.get 52
            local.get 64
            i32.add
            local.set 65
            local.get 58
            local.get 64
            i32.add
            local.set 66
            local.get 66
            i64.load align=4
            local.set 67
            local.get 65
            local.get 67
            i64.store align=4
            local.get 4
            i32.load offset=12
            local.set 68
            i32.const 1
            local.set 69
            local.get 68
            local.get 69
            i32.add
            local.set 70
            local.get 4
            local.get 70
            i32.store offset=12
            br 0 (;@4;)
          end
        end
        local.get 4
        i32.load offset=16
        local.set 71
        local.get 4
        i32.load offset=24
        local.set 72
        local.get 72
        local.get 71
        i32.store offset=8
        local.get 4
        i32.load offset=20
        local.set 73
        local.get 4
        i32.load offset=24
        local.set 74
        local.get 74
        local.get 73
        i32.store offset=16
      end
      local.get 4
      i32.load offset=24
      local.set 75
      local.get 75
      i32.load offset=8
      local.set 76
      local.get 4
      i32.load offset=24
      local.set 77
      local.get 77
      i32.load offset=12
      local.set 78
      i32.const 1
      local.set 79
      local.get 78
      local.get 79
      i32.add
      local.set 80
      local.get 77
      local.get 80
      i32.store offset=12
      i32.const 20
      local.set 81
      local.get 78
      local.get 81
      i32.mul
      local.set 82
      local.get 76
      local.get 82
      i32.add
      local.set 83
      local.get 1
      i64.load align=4
      local.set 84
      local.get 83
      local.get 84
      i64.store align=4
      i32.const 16
      local.set 85
      local.get 83
      local.get 85
      i32.add
      local.set 86
      local.get 1
      local.get 85
      i32.add
      local.set 87
      local.get 87
      i32.load
      local.set 88
      local.get 86
      local.get 88
      i32.store
      i32.const 8
      local.set 89
      local.get 83
      local.get 89
      i32.add
      local.set 90
      local.get 1
      local.get 89
      i32.add
      local.set 91
      local.get 91
      i64.load align=4
      local.set 92
      local.get 90
      local.get 92
      i64.store align=4
    end
    i32.const 32
    local.set 93
    local.get 4
    local.get 93
    i32.add
    local.set 94
    local.get 94
    global.set $__stack_pointer
    return)
  (func $dandelion_output_buffer_count (type 4) (param i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)
    global.get $__stack_pointer
    local.set 1
    i32.const 16
    local.set 2
    local.get 1
    local.get 2
    i32.sub
    local.set 3
    local.get 3
    local.get 0
    i32.store offset=8
    local.get 3
    i32.load offset=8
    local.set 4
    i32.const 0
    local.set 5
    local.get 5
    i32.load offset=1056
    local.set 6
    local.get 4
    local.set 7
    local.get 6
    local.set 8
    local.get 7
    local.get 8
    i32.ge_u
    local.set 9
    i32.const 1
    local.set 10
    local.get 9
    local.get 10
    i32.and
    local.set 11
    block  ;; label = @1
      block  ;; label = @2
        local.get 11
        i32.eqz
        br_if 0 (;@2;)
        i32.const 0
        local.set 12
        local.get 3
        local.get 12
        i32.store offset=12
        br 1 (;@1;)
      end
      i32.const 0
      local.set 13
      local.get 13
      i32.load offset=1028
      local.set 14
      local.get 3
      i32.load offset=8
      local.set 15
      i32.const 20
      local.set 16
      local.get 15
      local.get 16
      i32.mul
      local.set 17
      local.get 14
      local.get 17
      i32.add
      local.set 18
      local.get 18
      i32.load offset=12
      local.set 19
      local.get 3
      local.get 19
      i32.store offset=12
    end
    local.get 3
    i32.load offset=12
    local.set 20
    local.get 20
    return)
  (func $dandelion_input_set_ident (type 4) (param i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)
    global.get $__stack_pointer
    local.set 1
    i32.const 16
    local.set 2
    local.get 1
    local.get 2
    i32.sub
    local.set 3
    local.get 3
    local.get 0
    i32.store offset=8
    local.get 3
    i32.load offset=8
    local.set 4
    i32.const 0
    local.set 5
    local.get 5
    i32.load offset=1048
    local.set 6
    local.get 4
    local.set 7
    local.get 6
    local.set 8
    local.get 7
    local.get 8
    i32.ge_u
    local.set 9
    i32.const 1
    local.set 10
    local.get 9
    local.get 10
    i32.and
    local.set 11
    block  ;; label = @1
      block  ;; label = @2
        local.get 11
        i32.eqz
        br_if 0 (;@2;)
        i32.const 0
        local.set 12
        local.get 3
        local.get 12
        i32.store offset=12
        br 1 (;@1;)
      end
      i32.const 0
      local.set 13
      local.get 13
      i32.load offset=1024
      local.set 14
      local.get 3
      i32.load offset=8
      local.set 15
      i32.const 20
      local.set 16
      local.get 15
      local.get 16
      i32.mul
      local.set 17
      local.get 14
      local.get 17
      i32.add
      local.set 18
      local.get 18
      i32.load
      local.set 19
      local.get 3
      local.get 19
      i32.store offset=12
    end
    local.get 3
    i32.load offset=12
    local.set 20
    local.get 20
    return)
  (func $dandelion_input_set_ident_len (type 4) (param i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)
    global.get $__stack_pointer
    local.set 1
    i32.const 16
    local.set 2
    local.get 1
    local.get 2
    i32.sub
    local.set 3
    local.get 3
    local.get 0
    i32.store offset=8
    local.get 3
    i32.load offset=8
    local.set 4
    i32.const 0
    local.set 5
    local.get 5
    i32.load offset=1048
    local.set 6
    local.get 4
    local.set 7
    local.get 6
    local.set 8
    local.get 7
    local.get 8
    i32.ge_u
    local.set 9
    i32.const 1
    local.set 10
    local.get 9
    local.get 10
    i32.and
    local.set 11
    block  ;; label = @1
      block  ;; label = @2
        local.get 11
        i32.eqz
        br_if 0 (;@2;)
        i32.const 0
        local.set 12
        local.get 3
        local.get 12
        i32.store offset=12
        br 1 (;@1;)
      end
      i32.const 0
      local.set 13
      local.get 13
      i32.load offset=1024
      local.set 14
      local.get 3
      i32.load offset=8
      local.set 15
      i32.const 20
      local.set 16
      local.get 15
      local.get 16
      i32.mul
      local.set 17
      local.get 14
      local.get 17
      i32.add
      local.set 18
      local.get 18
      i32.load offset=4
      local.set 19
      local.get 3
      local.get 19
      i32.store offset=12
    end
    local.get 3
    i32.load offset=12
    local.set 20
    local.get 20
    return)
  (func $dandelion_output_set_ident (type 4) (param i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)
    global.get $__stack_pointer
    local.set 1
    i32.const 16
    local.set 2
    local.get 1
    local.get 2
    i32.sub
    local.set 3
    local.get 3
    local.get 0
    i32.store offset=8
    local.get 3
    i32.load offset=8
    local.set 4
    i32.const 0
    local.set 5
    local.get 5
    i32.load offset=1056
    local.set 6
    local.get 4
    local.set 7
    local.get 6
    local.set 8
    local.get 7
    local.get 8
    i32.ge_u
    local.set 9
    i32.const 1
    local.set 10
    local.get 9
    local.get 10
    i32.and
    local.set 11
    block  ;; label = @1
      block  ;; label = @2
        local.get 11
        i32.eqz
        br_if 0 (;@2;)
        i32.const 0
        local.set 12
        local.get 3
        local.get 12
        i32.store offset=12
        br 1 (;@1;)
      end
      i32.const 0
      local.set 13
      local.get 13
      i32.load offset=1028
      local.set 14
      local.get 3
      i32.load offset=8
      local.set 15
      i32.const 20
      local.set 16
      local.get 15
      local.get 16
      i32.mul
      local.set 17
      local.get 14
      local.get 17
      i32.add
      local.set 18
      local.get 18
      i32.load
      local.set 19
      local.get 3
      local.get 19
      i32.store offset=12
    end
    local.get 3
    i32.load offset=12
    local.set 20
    local.get 20
    return)
  (func $dandelion_output_set_ident_len (type 4) (param i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)
    global.get $__stack_pointer
    local.set 1
    i32.const 16
    local.set 2
    local.get 1
    local.get 2
    i32.sub
    local.set 3
    local.get 3
    local.get 0
    i32.store offset=8
    local.get 3
    i32.load offset=8
    local.set 4
    i32.const 0
    local.set 5
    local.get 5
    i32.load offset=1056
    local.set 6
    local.get 4
    local.set 7
    local.get 6
    local.set 8
    local.get 7
    local.get 8
    i32.ge_u
    local.set 9
    i32.const 1
    local.set 10
    local.get 9
    local.get 10
    i32.and
    local.set 11
    block  ;; label = @1
      block  ;; label = @2
        local.get 11
        i32.eqz
        br_if 0 (;@2;)
        i32.const 0
        local.set 12
        local.get 3
        local.get 12
        i32.store offset=12
        br 1 (;@1;)
      end
      i32.const 0
      local.set 13
      local.get 13
      i32.load offset=1028
      local.set 14
      local.get 3
      i32.load offset=8
      local.set 15
      i32.const 20
      local.set 16
      local.get 15
      local.get 16
      i32.mul
      local.set 17
      local.get 14
      local.get 17
      i32.add
      local.set 18
      local.get 18
      i32.load offset=4
      local.set 19
      local.get 3
      local.get 19
      i32.store offset=12
    end
    local.get 3
    i32.load offset=12
    local.set 20
    local.get 20
    return)
  (func $__dandelion_system_init (type 0)
    call $__dandelion_platform_init
    return)
  (func $__dandelion_system_exit (type 0)
    call $__dandelion_platform_exit
    return)
  (func $__dandelion_system_set_thread_pointer (type 3) (param i32)
    (local i32 i32 i32 i32 i32 i32)
    global.get $__stack_pointer
    local.set 1
    i32.const 16
    local.set 2
    local.get 1
    local.get 2
    i32.sub
    local.set 3
    local.get 3
    global.set $__stack_pointer
    local.get 3
    local.get 0
    i32.store offset=12
    local.get 3
    i32.load offset=12
    local.set 4
    local.get 4
    call $__dandelion_platform_set_thread_pointer
    i32.const 16
    local.set 5
    local.get 3
    local.get 5
    i32.add
    local.set 6
    local.get 6
    global.set $__stack_pointer
    return)
  (func $__dandelion_platform_init (type 0)
    return)
  (func $__dandelion_platform_exit (type 0)
    return)
  (func $__dandelion_platform_set_thread_pointer (type 3) (param i32)
    (local i32 i32 i32)
    global.get $__stack_pointer
    local.set 1
    i32.const 16
    local.set 2
    local.get 1
    local.get 2
    i32.sub
    local.set 3
    local.get 3
    local.get 0
    i32.store offset=12
    return)
  (memory (;0;) 2)
  (global $__stack_pointer (mut i32) (i32.const 66608))
  (global (;1;) i32 (i32.const 1036))
  (global (;2;) i32 (i32.const 1024))
  (global (;3;) i32 (i32.const 1024))
  (global (;4;) i32 (i32.const 1072))
  (global (;5;) i32 (i32.const 1024))
  (global (;6;) i32 (i32.const 66608))
  (global (;7;) i32 (i32.const 0))
  (global (;8;) i32 (i32.const 1))
  (export "memory" (memory 0))
  (export "__wasm_call_ctors" (func $__wasm_call_ctors))
  (export "_start" (func $_start))
  (export "__dandelion_entry" (func $__dandelion_entry))
  (export "mat_mul" (func $mat_mul))
  (export "dandelion_input_set_count" (func $dandelion_input_set_count))
  (export "dandelion_input_buffer_count" (func $dandelion_input_buffer_count))
  (export "dandelion_get_input" (func $dandelion_get_input))
  (export "dandelion_alloc" (func $dandelion_alloc))
  (export "dandelion_add_output" (func $dandelion_add_output))
  (export "dandelion_init" (func $dandelion_init))
  (export "dandelion_exit" (func $dandelion_exit))
  (export "__dandelion_system_init" (func $__dandelion_system_init))
  (export "__dandelion_system_data" (global 1))
  (export "__runtime_global_data" (global 2))
  (export "__dandelion_system_exit" (func $__dandelion_system_exit))
  (export "dandelion_set_thread_pointer" (func $dandelion_set_thread_pointer))
  (export "__dandelion_system_set_thread_pointer" (func $__dandelion_system_set_thread_pointer))
  (export "dandelion_output_set_count" (func $dandelion_output_set_count))
  (export "dandelion_output_buffer_count" (func $dandelion_output_buffer_count))
  (export "dandelion_input_set_ident" (func $dandelion_input_set_ident))
  (export "dandelion_input_set_ident_len" (func $dandelion_input_set_ident_len))
  (export "dandelion_output_set_ident" (func $dandelion_output_set_ident))
  (export "dandelion_output_set_ident_len" (func $dandelion_output_set_ident_len))
  (export "__dandelion_platform_init" (func $__dandelion_platform_init))
  (export "__dandelion_platform_exit" (func $__dandelion_platform_exit))
  (export "__dandelion_platform_set_thread_pointer" (func $__dandelion_platform_set_thread_pointer))
  (export "__dso_handle" (global 3))
  (export "__data_end" (global 4))
  (export "__global_base" (global 5))
  (export "__heap_base" (global 6))
  (export "__memory_base" (global 7))
  (export "__table_base" (global 8)))
