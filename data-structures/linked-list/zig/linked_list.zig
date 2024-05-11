const std = @import("std");
const testing = std.testing;

pub fn LinkedListEntry(comptime T: type) type {
    return struct {
        value: T,
        next: ?*LinkedListEntry(T) = null,
    };
}

pub fn LinkedList(comptime T: type) type {
    return struct {
        allocator: std.mem.Allocator,
        head: ?*LinkedListEntry(T) = null,

        const Self = @This();

        pub fn init(allocator: std.mem.Allocator) Self {
            return .{ .allocator = allocator };
        }

        pub fn deinit(self: *Self) void {
            var entry = self.head;
            while (entry != null) {
                const next = entry.?.next;
                self.allocator.destroy(entry.?);
                entry = next;
            }

            self.head = null;
        }

        pub fn pushFront(self: *Self, value: T) !void {
            const entry = try self.allocator.create(LinkedListEntry(T));
            entry.* = .{ .value = value, .next = self.head };
            self.head = entry;
        }

        pub fn push(self: *Self, value: T) !void {
            const entry = try self.allocator.create(LinkedListEntry(T));
            entry.* = .{ .value = value };

            if (self.head == null) {
                self.head = entry;
            } else {
                var last = self.head.?;
                while (last.next != null) : (last = last.next.?) {}
                last.next = entry;
            }
        }

        pub fn popFront(self: *Self) ?T {
            if (self.head == null) {
                return null;
            }

            const first = self.head.?;
            const value = first.value;

            self.head = first.next;

            self.allocator.destroy(first);
            return value;
        }

        pub fn pop(self: *Self) ?T {
            if (self.head == null) {
                return null;
            }

            var prev = self.head.?;
            var last = self.head.?;
            while (last.next != null) : (last = last.next.?) {
                prev = last;
            }

            const value = last.value;

            prev.next = null;
            self.allocator.destroy(last);

            return value;
        }

        pub fn contains(self: *Self, value: T) bool {
            var entry = self.head;
            while (entry != null) : (entry = entry.?.next) {
                if (entry.?.value == value) {
                    return true;
                }
            }

            return false;
        }
    };
}

test "push adds entries to the end" {
    const allocator = std.testing.allocator;

    var list = LinkedList(u32).init(allocator);
    defer list.deinit();

    try testing.expectEqual(list.head, null);

    try list.push(42);
    try list.push(23);

    try testing.expectEqual(list.head.?.value, 42);
    try testing.expectEqual(list.head.?.next.?.value, 23);
    try testing.expectEqual(list.head.?.next.?.next, null);
}

test "pushFront adds entries to the front" {
    const allocator = std.testing.allocator;

    var list = LinkedList(u32).init(allocator);
    defer list.deinit();

    try testing.expectEqual(list.head, null);

    try list.pushFront(42);
    try list.pushFront(23);

    try testing.expectEqual(list.head.?.value, 23);
    try testing.expectEqual(list.head.?.next.?.value, 42);
    try testing.expectEqual(list.head.?.next.?.next, null);
}

test "pop removes entries from the end and return value" {
    const allocator = std.testing.allocator;

    var list = LinkedList(u32).init(allocator);
    defer list.deinit();

    try testing.expectEqual(list.head, null);

    try list.push(42);
    try list.push(23);

    try testing.expectEqual(list.pop(), 23);

    try testing.expectEqual(list.head.?.value, 42);
    try testing.expectEqual(list.head.?.next, null);
}

test "popFront adds entries to the front and return value" {
    const allocator = std.testing.allocator;

    var list = LinkedList(u32).init(allocator);
    defer list.deinit();

    try testing.expectEqual(list.head, null);

    try list.push(42);
    try list.push(23);

    try testing.expectEqual(list.popFront(), 42);

    try testing.expectEqual(list.head.?.value, 23);
    try testing.expectEqual(list.head.?.next, null);
}

test "contains" {
    const allocator = std.testing.allocator;

    var list = LinkedList(u32).init(allocator);
    defer list.deinit();

    try testing.expectEqual(list.head, null);
    try testing.expect(!list.contains(42));

    try list.push(42);
    try list.push(23);

    try testing.expect(list.contains(42));
    try testing.expect(list.contains(23));

    try testing.expect(!list.contains(5));
}
