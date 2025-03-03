#include <bits/stdc++.h>

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;
using u128 = unsigned __int128;

template<class T>
constexpr T power(T a, u64 b, T res = 1) {
    for (; b != 0; b /= 2, a *= a) {
        if (b & 1) {
            res *= a;
        }
    }
    return res;
}

template<u32 P>
constexpr u32 mulMod(u32 a, u32 b) {
    return u64(a) * b % P;
}

template<u64 P>
constexpr u64 mulMod(u64 a, u64 b) {
    u64 res = a * b - u64(1.L * a * b / P - 0.5L) * P;
    res %= P;
    return res;
}

constexpr i64 safeMod(i64 x, i64 m) {
    x %= m;
    if (x < 0) {
        x += m;
    }
    return x;
}

constexpr std::pair<i64, i64> invGcd(i64 a, i64 b) {
    a = safeMod(a, b);
    if (a == 0) {
        return {b, 0};
    }
    
    i64 s = b, t = a;
    i64 m0 = 0, m1 = 1;

    while (t) {
        i64 u = s / t;
        s -= t * u;
        m0 -= m1 * u;
        
        std::swap(s, t);
        std::swap(m0, m1);
    }
    
    if (m0 < 0) {
        m0 += b / s;
    }
    
    return {s, m0};
}

template<std::unsigned_integral U, U P>
struct ModIntBase {
public:
    constexpr ModIntBase() : x(0) {}
    template<std::unsigned_integral T>
    constexpr ModIntBase(T x_) : x(x_ % mod()) {}
    template<std::signed_integral T>
    constexpr ModIntBase(T x_) {
        using S = std::make_signed_t<U>;
        S v = x_;
        v %= S(mod());
        if (v < 0) {
            v += mod();
        }
        x = v;
    }
    
    constexpr static U mod() {
        return P;
    }
    
    constexpr U val() const {
        return x;
    }
    
    constexpr ModIntBase operator-() const {
        ModIntBase res;
        res.x = (x == 0 ? 0 : mod() - x);
        return res;
    }
    
    constexpr ModIntBase inv() const {
        return power(*this, mod() - 2);
    }
    
    constexpr ModIntBase &operator*=(const ModIntBase &rhs) & {
        x = mulMod<mod()>(x, rhs.val());
        return *this;
    }
    constexpr ModIntBase &operator+=(const ModIntBase &rhs) & {
        x += rhs.val();
        if (x >= mod()) {
            x -= mod();
        }
        return *this;
    }
    constexpr ModIntBase &operator-=(const ModIntBase &rhs) & {
        x -= rhs.val();
        if (x >= mod()) {
            x += mod();
        }
        return *this;
    }
    constexpr ModIntBase &operator/=(const ModIntBase &rhs) & {
        return *this *= rhs.inv();
    }
    
    friend constexpr ModIntBase operator*(ModIntBase lhs, const ModIntBase &rhs) {
        lhs *= rhs;
        return lhs;
    }
    friend constexpr ModIntBase operator+(ModIntBase lhs, const ModIntBase &rhs) {
        lhs += rhs;
        return lhs;
    }
    friend constexpr ModIntBase operator-(ModIntBase lhs, const ModIntBase &rhs) {
        lhs -= rhs;
        return lhs;
    }
    friend constexpr ModIntBase operator/(ModIntBase lhs, const ModIntBase &rhs) {
        lhs /= rhs;
        return lhs;
    }
    
    friend constexpr std::istream &operator>>(std::istream &is, ModIntBase &a) {
        i64 i;
        is >> i;
        a = i;
        return is;
    }
    friend constexpr std::ostream &operator<<(std::ostream &os, const ModIntBase &a) {
        return os << a.val();
    }
    
    friend constexpr std::strong_ordering operator<=>(ModIntBase lhs, ModIntBase rhs) {
        return lhs.val() <=> rhs.val();
    }
    
private:
    U x;
};

template<u32 P>
using ModInt = ModIntBase<u32, P>;
template<u64 P>
using ModInt64 = ModIntBase<u64, P>;

struct Barrett {
public:
    Barrett(u32 m_) : m(m_), im((u64)(-1) / m_ + 1) {}

    constexpr u32 mod() const {
        return m;
    }

    constexpr u32 mul(u32 a, u32 b) const {
        u64 z = a;
        z *= b;
        
        u64 x = u64((u128(z) * im) >> 64);
        
        u32 v = u32(z - x * m);
        if (m <= v) {
            v += m;
        }
        return v;
    }

private:
    u32 m;
    u64 im;
};

template<u32 Id>
struct DynModInt {
public:
    constexpr DynModInt() : x(0) {}
    template<std::unsigned_integral T>
    constexpr DynModInt(T x_) : x(x_ % mod()) {}
    template<std::signed_integral T>
    constexpr DynModInt(T x_) {
        int v = x_;
        v %= int(mod());
        if (v < 0) {
            v += mod();
        }
        x = v;
    }
    
    constexpr static void setMod(u32 m) {
        bt = m;
    }
    
    static u32 mod() {
        return bt.mod();
    }
    
    constexpr u32 val() const {
        return x;
    }
    
    constexpr DynModInt operator-() const {
        DynModInt res;
        res.x = (x == 0 ? 0 : mod() - x);
        return res;
    }
    
    constexpr DynModInt inv() const {
        auto v = invGcd(x, mod());
        assert(v.first == 1);
        return v.second;
    }
    
    constexpr DynModInt &operator*=(const DynModInt &rhs) & {
        x = bt.mul(x, rhs.val());
        return *this;
    }
    constexpr DynModInt &operator+=(const DynModInt &rhs) & {
        x += rhs.val();
        if (x >= mod()) {
            x -= mod();
        }
        return *this;
    }
    constexpr DynModInt &operator-=(const DynModInt &rhs) & {
        x -= rhs.val();
        if (x >= mod()) {
            x += mod();
        }
        return *this;
    }
    constexpr DynModInt &operator/=(const DynModInt &rhs) & {
        return *this *= rhs.inv();
    }
    
    friend constexpr DynModInt operator*(DynModInt lhs, const DynModInt &rhs) {
        lhs *= rhs;
        return lhs;
    }
    friend constexpr DynModInt operator+(DynModInt lhs, const DynModInt &rhs) {
        lhs += rhs;
        return lhs;
    }
    friend constexpr DynModInt operator-(DynModInt lhs, const DynModInt &rhs) {
        lhs -= rhs;
        return lhs;
    }
    friend constexpr DynModInt operator/(DynModInt lhs, const DynModInt &rhs) {
        lhs /= rhs;
        return lhs;
    }
    
    friend constexpr std::istream &operator>>(std::istream &is, DynModInt &a) {
        i64 i;
        is >> i;
        a = i;
        return is;
    }
    friend constexpr std::ostream &operator<<(std::ostream &os, const DynModInt &a) {
        return os << a.val();
    }
    
    friend constexpr std::strong_ordering operator<=>(DynModInt lhs, DynModInt rhs) {
        return lhs.val() <=> rhs.val();
    }
    
private:
    u32 x;
    static Barrett bt;
};

template<u32 Id>
Barrett DynModInt<Id>::bt = 998244353;
using Z = ModInt<998244353>;

void update(std::pair<int, Z> &a, std::pair<int, Z> b) {
    if (b.first > a.first) {
        a = b;
    } else if (b.first == a.first) {
        a.second += b.second;
    }
}
void solve() {
    int n, m;
    std::cin >> n >> m;
    
    std::vector<int> a(n, -1);
    for (int i = 0; i < m; i++) {
        int x, k;
        std::cin >> x >> k;
        x--;
        k--;
        a[x] = k;
    }
    
    // if (n > 1000) {
    //     return;
    // }
    
    std::vector<std::pair<int, Z>> dp(n);
    
    std::pair<int, Z> ans {0, 1};
    std::vector<Z> pw(n + 1);
    pw[0] = 1;
    for (int i = 1; i <= n; i++) {
        pw[i] = pw[i - 1] * n;
    }
    std::vector<int> dep(n);
    std::vector<int> depl(n), depr(n), rd(n), par(n, -1);
    std::vector<int> lc(n, -1), rc(n, -1);
    
    std::vector<bool> vis(n);
    
    auto dfs1 = [&](this auto &&self, int l, int r, int p = -1, bool left = false) -> int {
        if (l > r) {
            return -1;
        }
        if (l == r && vis[l]) {
            return -1;
        }
        int m = (l + r) / 2;
        vis[m] = true;
        par[m] = p;
        dep[m] = (p == -1 ? 0 : dep[p] + 1);
        depl[m] = (p == -1 ? 0 : depl[p] + (m < p));
        depr[m] = (p == -1 ? 0 : depr[p] + (m > p));
        lc[m] = self(l, m, m, true);
        rc[m] = self(m + 1, r, m);
        rd[m] = rc[m] == -1 ? 0 : 1 + rd[rc[m]];
        return m;
    };
    dfs1(0, n - 1);
    
    vis.assign(n, false);
    
    auto dfs = [&](this auto &&self, int l, int r, int p = -1, bool left = false) -> int {
        if (l > r) {
            return -1;
        }
        if (l == r && vis[l]) {
            return -1;
        }
        int m = (l + r) / 2;
        vis[m] = true;
        self(l, m, m, true);
        if (a[m] != -1) {
            int lt = (lc[m] == -1 ? 0 : 1 + rd[lc[m]]) + depr[m];
            assert(m - lt >= 0);
            if (a[m] > 0 || lt == 0) {
                update(dp[m], std::make_pair(1, power(Z(a[m]), lt) * power(Z(n), m - lt)));
            }
            
            for (int i = l; i < m; i++) {
                if (a[i] != -1 && a[i] < a[m] && dp[i].first != 0) {
                    // std::cerr << "(" << i << ", " << m << ")\n";
                    int u = i;
                    while (depl[u] != depl[m] + 1) {
                        u = par[u];
                    }
                    int lt = rd[u];
                    int bt = (i != u);
                    int ge = depl[i] - depl[u] - bt;
                    assert(m - i - 1 - lt - bt - ge >= 0);
                    update(dp[m], std::make_pair(dp[i].first + 1, dp[i].second * power(Z(a[m]), lt) * power(Z(a[m] - a[i]), bt)
                        * power(Z(n - a[i]), ge) * power(Z(n), m - i - 1 - lt - bt - ge)));
                    // std::cerr << power(Z(a[m]), lt) * power(Z(a[m] - a[i]), bt)
                    //     * power(Z(n - a[i]), ge) * power(Z(n), m - i - 1 - lt - bt - ge) << "\n";
                }
            }
            for (int j = m + 1; j <= r; j++) {
                if (dep[j] > dep[m] && a[j] != -1 && a[m] < a[j]) {
                    // std::cerr << "(" << m << ", " << j << ")\n";
                    int lt = (lc[j] == -1 ? 0 : 1 + rd[lc[j]]) + depr[j] - depr[m] - 1;
                    assert(lt >= 0);
                    assert(j - m - 1 - lt >= 0);
                    update(dp[j], std::make_pair(dp[m].first + 1, dp[m].second * power(Z(a[j]), lt) * power(Z(n), j - m - 1 - lt)));
                    // std::cerr << power(Z(a[j]), lt) * power(Z(n), j - m - 1 - lt) << "\n";
                }
            }
        }
        for (int i = l; i < m; i++) {
            if (a[i] != -1 && dp[i].first != 0) {
                for (int j = m + 1; j <= r; j++) {
                    if (dep[j] > dep[m] && a[j] != -1 && a[j] > a[i]) {
                        // std::cerr << "(" << i << ", " << j << ")\n";
                        int gei = depl[i] - depl[m] - 1;
                        int ltj = (lc[j] == -1 ? 0 : 1 + rd[lc[j]]) + depr[j] - depr[m] - 1;
                        assert(ltj >= 0);
                        assert(j - i - 1 - gei - ltj - 1 >= 0);
                        
                        update(dp[j], std::make_pair(dp[i].first + 1, dp[i].second * power(Z(n - a[i]), gei) * power(Z(a[j]), ltj)
                                * (a[j] - a[i]) * power(Z(n), j - i - 1 - gei - ltj - 1)));
                        // std::cerr << dp[i].second * power(Z(a[i]), gei) * power(Z(a[j]), ltj)
                        //         * (a[j] - a[i]) * power(Z(n), j - i - 1 - gei - ltj - 1) << "\n";
                    }
                }
            }
        }
        self(m + 1, r, m);
        return m;
    };
    dfs(0, n - 1);
    
    for (int i = 0; i < n; i++) {
        if (a[i] != -1 && dp[i].first != 0) {
            // std::cerr << i << " " << a[i] << " " << dp[i].first << " " << dp[i].second << "\n";
            int ge = depl[i];
            assert(n - 1 - i - ge >= 0);
            update(ans, std::make_pair(dp[i].first, dp[i].second * power(Z(n - a[i]), ge) * power(Z(n), n - 1 - i - ge)));
        }
    }
    
    std::cout << m - ans.first << " " << ans.second << "\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int t;
    std::cin >> t;
    
    while (t--) {
        solve();
    }
    
    return 0;
}