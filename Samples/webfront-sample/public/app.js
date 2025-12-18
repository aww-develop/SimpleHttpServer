// UE 側の SimpleHttpServer のポートに合わせる
const BASE_URL = "http://127.0.0.1:9120";
console.log("API Base URL:", BASE_URL);

document.addEventListener("DOMContentLoaded", () => {
    const el = document.getElementById("baseUrlText");
    if (el) el.textContent = BASE_URL;
});

document.addEventListener("htmx:configRequest", (e) => {
    const d = e.detail;
    const elt = d.elt; // リクエストを発火した要素（button/form等）

    // 1) /user/:id/profile だけ特別処理
    if (elt && elt.getAttribute("data-user-profile-request") === "1") {
        const selector = elt.getAttribute("data-user-id-input");
        const input = selector ? document.querySelector(selector) : null;
        const id = (input?.value ?? "").trim();

        if (!id) {
            e.preventDefault();
            const out = document.getElementById("userProfileResult");
            if (out) out.textContent = "Please enter a user id.";
            return;
        }

        // ここで相対パスに差し替える（後段で BASE_URL を付ける）
        d.path = `/user/${encodeURIComponent(id)}/profile`;
    }

    // 2) 相対パス (/version /echo /user/...) を BASE_URL に寄せる
    if (d.path && d.path.startsWith("/")) {
        d.path = BASE_URL + d.path;
    }
});

// 送信前の入力チェック（空なら止める）
window.validateEcho = function validateEcho(event) {
    const input = document.getElementById("echoInput");
    const msg = (input?.value ?? "").trim();
    const out = document.getElementById("echoResult");

    if (!msg) {
        if (out) out.textContent = "Please enter a message.";
        event.preventDefault();
    }
};
