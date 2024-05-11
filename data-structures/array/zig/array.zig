const std = @import("std");
const testing = std.testing;

pub fn Array(comptime T: type) type {
    return struct {
        allocator: std.mem.Allocator,
        entries: ?[]T = null, // TODO: not 100% sure about that...
        count: usize = 0,
        capacity: usize = 0,

        const Self = @This();

        pub fn init(allocator: std.mem.Allocator) Self {
            return .{ .allocator = allocator };
        }

        pub fn deinit(self: *Self) void {
            if (self.entries != null) {
                self.allocator.free(self.entries.?);
                self.entries = null;
            }

            self.count = 0;
            self.capacity = 0;
        }

        pub fn add(self: *Self, value: T) !void {
            if (self.count >= self.capacity) {
                if (self.capacity == 0) {
                    self.capacity = 8;
                    self.entries = try self.allocator.alloc(T, self.capacity);
                } else {
                    self.capacity = self.capacity * 2;
                    self.entries = try self.allocator.realloc(self.entries.?, self.capacity);
                }
            }

            self.entries.?[self.count] = value;
            self.count += 1;
        }

        pub fn get(self: *Self, idx: usize) ?T {
            if (idx >= self.count) {
                return null;
            }

            return self.entries.?[idx];
        }
    };
}

test "add" {
    const allocator = std.testing.allocator;

    var array = Array(u32).init(allocator);
    defer array.deinit();

    try testing.expectEqual(array.get(0), null);

    try array.add(42);
    try array.add(23);

    try testing.expectEqual(array.count, 2);
    try testing.expectEqual(array.capacity, 8);
    try testing.expectEqual(array.get(0), 42);
    try testing.expectEqual(array.get(1), 23);
    try testing.expectEqual(array.get(2), null);

    for (0..1000) |i| {
        try array.add(@intCast(i));
    }
    try testing.expectEqual(array.count, 1002);
    try testing.expectEqual(array.get(1001), 999);
}
