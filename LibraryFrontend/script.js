// ---------- DATA ----------

let currentUser = null;

const books = [
  { id: 1, title: "The Great Gatsby", author: "F. Scott Fitzgerald", category: "Classic", status: "available" },
  { id: 2, title: "1984", author: "George Orwell", category: "Fiction", status: "borrowed" },
  { id: 3, title: "To Kill a Mockingbird", author: "Harper Lee", category: "Fiction", status: "available" },
  { id: 4, title: "Sapiens", author: "Yuval Noah Harari", category: "Non-fiction", status: "available" },
  { id: 5, title: "Atomic Habits", author: "James Clear", category: "Self-help", status: "available" }
];

const laptops = [
  { id: 1, model: "MacBook Air M2", serial: "MBAM2-1032", status: "available" },
  { id: 2, model: "Dell XPS 13", serial: "XPS13-7781", status: "borrowed" },
  { id: 3, model: "Lenovo ThinkPad X1", serial: "X1-4421", status: "available" },
  { id: 4, model: "HP Spectre x360", serial: "HPS-8830", status: "available" }
];

const rooms = [
  { id: 1, name: "Room A", capacity: 6, type: "Screen, whiteboard" },
  { id: 2, name: "Room B", capacity: 10, type: "Conference, HDMI" },
  { id: 3, name: "Room C", capacity: 4, type: "Quiet study" },
  { id: 4, name: "Room D", capacity: 8, type: "Whiteboard" }
];

const roomSlots = [
  "08:00-09:00",
  "09:00-10:00",
  "10:00-11:00",
  "11:00-12:00",
  "12:00-13:00",
  "13:00-14:00",
  "14:00-15:00",
  "15:00-16:00",
  "16:00-17:00",
  "17:00-18:00",
  "18:00-19:00",
  "19:00-20:00"
];

// Bookings: date -> { roomId -> Set(slots) }
const bookings = {};
const activityLog = [];
const wishlist = [];

// ---------- HELPERS ----------

function $(selector) {
  return document.querySelector(selector);
}

const todayISO = () => new Date().toISOString().slice(0, 10);

function ensureRoomDate() {
  const input = $("#roomDate");
  if (input && !input.value) input.value = todayISO();
}

function requireLogin() {
  if (currentUser) return true;
  alert("Please log in or sign up first.");
  openAuthModal();
  return false;
}

function isInWishlist(bookId) {
  return wishlist.some((b) => b.id === bookId);
}

function addToWishlist(book) {
  if (!requireLogin()) return;
  if (isInWishlist(book.id)) return;
  wishlist.push(book);
  activityLog.push({
    type: "wishlist",
    title: book.title,
    meta: `Added to wishlist by ${currentUser.email}`,
    timestamp: new Date()
  });
  renderWishlist();
  renderActivity();
}

function removeFromWishlist(bookId) {
  const index = wishlist.findIndex((b) => b.id === bookId);
  if (index !== -1) wishlist.splice(index, 1);
  renderWishlist();
}

// ---------- NAV ----------

function showSection(id) {
  document.querySelectorAll(".section").forEach((s) => {
    s.classList.toggle("visible", s.id === id);
  });
  document.querySelectorAll(".nav-link").forEach((b) => {
    b.classList.toggle("active", b.dataset.section === id);
  });
}

function setupNavigation() {
  document.querySelectorAll(".nav-link").forEach((btn) => {
    btn.addEventListener("click", () => showSection(btn.dataset.section));
  });

  document.querySelectorAll("[data-section-target]").forEach((btn) => {
    btn.addEventListener("click", () => {
      const target = btn.dataset.sectionTarget;
      if (target) showSection(target);
    });
  });
}

// ---------- BOOKS ----------

function renderBooks(filter = "") {
  const container = $("#bookResults");
  const empty = $("#bookEmpty");
  container.innerHTML = "";

  const q = filter.trim().toLowerCase();
  const filtered = books.filter(
    (b) => !q || b.title.toLowerCase().includes(q) || b.author.toLowerCase().includes(q)
  );

  if (!filtered.length && q) {
    empty.classList.remove("hidden");
    empty.innerHTML = `
      <h3>Book not available</h3>
      <p>We couldn't find any book with "<strong>${filter}</strong>".</p>
      <p>Here are some other books you might like:</p>
    `;

    const suggestions = books
      .filter((b) => b.status === "available")
      .sort(() => Math.random() - 0.5)
      .slice(0, 3);

    suggestions.forEach((book) => {
      const card = document.createElement("article");
      card.className = "card";

      const header = document.createElement("div");
      header.className = "card-header-line";

      const titleBox = document.createElement("div");
      titleBox.innerHTML = `
        <div class="card-title">${book.title}</div>
        <div class="card-subtitle">${book.author}</div>
      `;

      const badge = document.createElement("span");
      badge.className = "badge available";
      badge.textContent = "Available";

      header.appendChild(titleBox);
      header.appendChild(badge);

      const meta = document.createElement("div");
      meta.className = "card-meta";
      meta.innerHTML = `<span>Category: ${book.category}</span><span>ID: #${book.id}</span>`;

      const footer = document.createElement("div");
      footer.className = "card-footer";

      const detailsBtn = document.createElement("button");
      detailsBtn.className = "ghost";
      detailsBtn.textContent = "Details";
      detailsBtn.addEventListener("click", () =>
        openDetailsModal(
          "Book Details",
          `
            <strong>${book.title}</strong><br/>
            Author: ${book.author}<br/>
            Category: ${book.category}<br/>
            Status: ${book.status}
          `
        )
      );

      const wishlistBtn = document.createElement("button");
      wishlistBtn.className = "secondary";
      if (isInWishlist(book.id)) {
        wishlistBtn.textContent = "In wishlist";
        wishlistBtn.disabled = true;
      } else {
        wishlistBtn.textContent = "Add to wishlist";
        wishlistBtn.addEventListener("click", () => addToWishlist(book));
      }

      const borrowBtn = document.createElement("button");
      borrowBtn.className = "primary";
      borrowBtn.textContent = "Borrow";
      borrowBtn.addEventListener("click", () => {
        if (!requireLogin()) return;
        book.status = "borrowed";
        activityLog.push({
          type: "book",
          title: book.title,
          meta: `Borrowed by ${currentUser.email}`,
          timestamp: new Date()
        });
        renderBooks("");
        renderWishlist();
        renderActivity();
        renderAdminPanels();
      });

      footer.appendChild(detailsBtn);
      footer.appendChild(wishlistBtn);
      footer.appendChild(borrowBtn);

      card.appendChild(header);
      card.appendChild(meta);
      card.appendChild(footer);
      empty.appendChild(card);
    });

    return;
  }

  if (!filtered.length && !q) {
    empty.classList.remove("hidden");
    empty.innerHTML = `
      <h3>No books in the system</h3>
      <p>Add books to the catalogue to see them here.</p>
    `;
    return;
  }

  empty.classList.add("hidden");

  filtered.forEach((book) => {
    const card = document.createElement("article");
    card.className = "card";

    const header = document.createElement("div");
    header.className = "card-header-line";

    const titleBox = document.createElement("div");
    titleBox.innerHTML = `
      <div class="card-title">${book.title}</div>
      <div class="card-subtitle">${book.author}</div>
    `;

    const badge = document.createElement("span");
    badge.className = "badge " + (book.status === "available" ? "available" : "unavailable");
    badge.textContent = book.status === "available" ? "Available" : "Checked out";

    header.appendChild(titleBox);
    header.appendChild(badge);

    const meta = document.createElement("div");
    meta.className = "card-meta";
    meta.innerHTML = `<span>Category: ${book.category}</span><span>ID: #${book.id}</span>`;

    const footer = document.createElement("div");
    footer.className = "card-footer";

    const detailsBtn = document.createElement("button");
    detailsBtn.className = "ghost";
    detailsBtn.textContent = "Details";
    detailsBtn.addEventListener("click", () =>
      openDetailsModal(
        "Book Details",
        `
          <strong>${book.title}</strong><br/>
          Author: ${book.author}<br/>
          Category: ${book.category}<br/>
          Status: ${book.status}
        `
      )
    );

    const wishlistBtn = document.createElement("button");
    wishlistBtn.className = "secondary";
    if (isInWishlist(book.id)) {
      wishlistBtn.textContent = "In wishlist";
      wishlistBtn.disabled = true;
    } else {
      wishlistBtn.textContent = "Add to wishlist";
      wishlistBtn.addEventListener("click", () => addToWishlist(book));
    }

    const borrowBtn = document.createElement("button");
    borrowBtn.className = "primary";
    borrowBtn.textContent = book.status === "available" ? "Borrow" : "Borrowed";
    borrowBtn.disabled = book.status !== "available";
    borrowBtn.addEventListener("click", () => {
      if (!requireLogin()) return;
      if (book.status !== "available") return;

      book.status = "borrowed";
      activityLog.push({
        type: "book",
        title: book.title,
        meta: `Borrowed by ${currentUser.email}`,
        timestamp: new Date()
      });
      renderBooks(filter);
      renderWishlist();
      renderActivity();
      renderAdminPanels();
    });

    footer.appendChild(detailsBtn);
    footer.appendChild(wishlistBtn);
    footer.appendChild(borrowBtn);

    card.appendChild(header);
    card.appendChild(meta);
    card.appendChild(footer);
    container.appendChild(card);
  });
}

// ---------- WISHLIST ----------

function renderWishlist() {
  const list = $("#wishlistList");
  const empty = $("#wishlistEmpty");
  list.innerHTML = "";

  if (!wishlist.length) {
    empty.classList.remove("hidden");
    return;
  }
  empty.classList.add("hidden");

  wishlist.forEach((book) => {
    const card = document.createElement("article");
    card.className = "card";

    const header = document.createElement("div");
    header.className = "card-header-line";

    const titleBox = document.createElement("div");
    titleBox.innerHTML = `
      <div class="card-title">${book.title}</div>
      <div class="card-subtitle">${book.author}</div>
    `;

    const badge = document.createElement("span");
    badge.className = "badge " + (book.status === "available" ? "available" : "unavailable");
    badge.textContent = book.status === "available" ? "Available" : "Checked out";

    header.appendChild(titleBox);
    header.appendChild(badge);

    const meta = document.createElement("div");
    meta.className = "card-meta";
    meta.innerHTML = `<span>Category: ${book.category}</span><span>ID: #${book.id}</span>`;

    const footer = document.createElement("div");
    footer.className = "card-footer";

    const removeBtn = document.createElement("button");
    removeBtn.className = "ghost danger";
    removeBtn.textContent = "Remove";
    removeBtn.addEventListener("click", () => {
      removeFromWishlist(book.id);
    });

    const borrowBtn = document.createElement("button");
    borrowBtn.className = "primary";
    borrowBtn.textContent = book.status === "available" ? "Borrow" : "Borrowed";
    borrowBtn.disabled = book.status !== "available";
    borrowBtn.addEventListener("click", () => {
      if (!requireLogin()) return;
      if (book.status !== "available") return;

      book.status = "borrowed";
      activityLog.push({
        type: "book",
        title: book.title,
        meta: `Borrowed (from wishlist) by ${currentUser.email}`,
        timestamp: new Date()
      });
      removeFromWishlist(book.id);
      renderBooks();
      renderWishlist();
      renderActivity();
      renderAdminPanels();
    });

    footer.appendChild(removeBtn);
    footer.appendChild(borrowBtn);

    card.appendChild(header);
    card.appendChild(meta);
    card.appendChild(footer);
    list.appendChild(card);
  });
}

// ---------- LAPTOPS ----------

function renderLaptops(filter = "") {
  const container = $("#laptopGrid");
  const empty = $("#laptopEmpty");
  container.innerHTML = "";

  const q = filter.trim().toLowerCase();
  const filtered = laptops.filter(
    (l) => !q || l.model.toLowerCase().includes(q) || l.serial.toLowerCase().includes(q)
  );

  if (!filtered.length) {
    empty.classList.remove("hidden");
    return;
  }
  empty.classList.add("hidden");

  filtered.forEach((lap) => {
    const card = document.createElement("article");
    card.className = "card";

    const header = document.createElement("div");
    header.className = "card-header-line";

    const titleBox = document.createElement("div");
    titleBox.innerHTML = `
      <div class="card-title">${lap.model}</div>
      <div class="card-subtitle">Serial: ${lap.serial}</div>
    `;

    const badge = document.createElement("span");
    badge.className = "badge " + (lap.status === "available" ? "available" : "unavailable");
    badge.textContent = lap.status === "available" ? "Available" : "Checked out";

    header.appendChild(titleBox);
    header.appendChild(badge);

    const meta = document.createElement("div");
    meta.className = "card-meta";
    meta.innerHTML = `<span>Asset ID: LP-${lap.id.toString().padStart(3, "0")}</span>`;

    const footer = document.createElement("div");
    footer.className = "card-footer";

    const statusBtn = document.createElement("button");
    statusBtn.className = "ghost";
    statusBtn.textContent = lap.status === "available" ? "Details" : "Return";
    statusBtn.addEventListener("click", () => {
      if (lap.status === "available") {
        openDetailsModal(
          "Laptop Details",
          `
          <strong>${lap.model}</strong><br />
          Serial: ${lap.serial}<br />
          Status: ${lap.status}
        `
        );
      } else {
        if (!requireLogin()) return;
        lap.status = "available";
        activityLog.push({
          type: "laptop",
          title: lap.model,
          meta: `Returned by ${currentUser.email}`,
          timestamp: new Date()
        });
        renderLaptops(filter);
        renderActivity();
        renderAdminPanels();
      }
    });

    const borrowBtn = document.createElement("button");
    borrowBtn.className = "primary";
    borrowBtn.textContent = lap.status === "available" ? "Borrow" : "Borrowed";
    borrowBtn.disabled = lap.status !== "available";
    borrowBtn.addEventListener("click", () => {
      if (!requireLogin()) return;
      if (lap.status !== "available") return;

      lap.status = "borrowed";
      activityLog.push({
        type: "laptop",
        title: lap.model,
        meta: `Borrowed by ${currentUser.email}`,
        timestamp: new Date()
      });
      renderLaptops(filter);
      renderActivity();
      renderAdminPanels();
    });

    footer.appendChild(statusBtn);
    footer.appendChild(borrowBtn);

    card.appendChild(header);
    card.appendChild(meta);
    card.appendChild(footer);
    container.appendChild(card);
  });
}

// ---------- ROOMS ----------

function getDateBookings(dateISO) {
  if (!bookings[dateISO]) bookings[dateISO] = {};
  return bookings[dateISO];
}

function isSlotBooked(roomId, dateISO, slot) {
  const dateBookings = getDateBookings(dateISO);
  const roomBooking = dateBookings[roomId];
  return roomBooking ? roomBooking.has(slot) : false;
}

function addBooking(roomId, dateISO, slot) {
  const dateBookings = getDateBookings(dateISO);
  if (!dateBookings[roomId]) dateBookings[roomId] = new Set();
  dateBookings[roomId].add(slot);
}

function renderRooms() {
  const dateISO = $("#roomDate").value || todayISO();
  const container = $("#roomGrid");
  const empty = $("#roomEmpty");
  container.innerHTML = "";

  if (!rooms.length) {
    empty.classList.remove("hidden");
    return;
  }
  empty.classList.add("hidden");

  rooms.forEach((room) => {
    const card = document.createElement("article");
    card.className = "card";

    const header = document.createElement("div");
    header.className = "card-header-line";

    const titleBox = document.createElement("div");
    titleBox.innerHTML = `
      <div class="card-title">${room.name}</div>
      <div class="card-subtitle">${room.type}</div>
    `;

    const badge = document.createElement("span");
    badge.className = "badge available";
    badge.textContent = `${room.capacity} ppl`;

    header.appendChild(titleBox);
    header.appendChild(badge);

    const meta = document.createElement("div");
    meta.className = "card-meta";
    meta.innerHTML = `<span>Date: ${dateISO}</span>`;

    const slotGrid = document.createElement("div");
    slotGrid.className = "slot-grid";

    roomSlots.forEach((slot) => {
      const btn = document.createElement("button");
      btn.className = "slot-btn";
      btn.textContent = slot;

      const booked = isSlotBooked(room.id, dateISO, slot);
      if (booked) btn.classList.add("disabled");

      btn.addEventListener("click", () => {
        if (!requireLogin()) return;
        if (isSlotBooked(room.id, dateISO, slot)) return;

        addBooking(room.id, dateISO, slot);
        activityLog.push({
          type: "room",
          title: `${room.name} – ${slot}`,
          meta: `Reserved on ${dateISO} by ${currentUser.email}`,
          timestamp: new Date()
        });

        renderRooms();
        renderActivity();
        renderAdminPanels();
      });

      slotGrid.appendChild(btn);
    });

    card.appendChild(header);
    card.appendChild(meta);
    card.appendChild(slotGrid);
    container.appendChild(card);
  });
}

// ---------- ACTIVITY ----------

function renderActivity() {
  const list = $("#activityList");
  const empty = $("#activityEmpty");
  list.innerHTML = "";

  if (!activityLog.length) {
    empty.classList.remove("hidden");
    return;
  }
  empty.classList.add("hidden");

  activityLog
    .slice()
    .sort((a, b) => b.timestamp - a.timestamp)
    .forEach((item, index) => {
      const row = document.createElement("div");
      row.className = "activity-item";

      const main = document.createElement("div");
      main.className = "activity-main";
      main.innerHTML = `
        <div class="activity-type">${item.type.toUpperCase()}</div>
        <div class="activity-title">${item.title}</div>
        <div class="activity-meta">${item.meta}</div>
      `;

      const actions = document.createElement("div");
      actions.className = "activity-actions";

      const viewBtn = document.createElement("button");
      viewBtn.className = "ghost";
      viewBtn.textContent = "View";
      viewBtn.addEventListener("click", () =>
        openDetailsModal(
          "Activity details",
          `
          <strong>Type:</strong> ${item.type}<br />
          <strong>Title:</strong> ${item.title}<br />
          <strong>Meta:</strong> ${item.meta}<br />
          <strong>Time:</strong> ${item.timestamp.toLocaleString()}
        `
        )
      );

      const delBtn = document.createElement("button");
      delBtn.className = "ghost danger";
      delBtn.textContent = "Remove";
      delBtn.addEventListener("click", () => {
        if (!requireLogin()) return;
        activityLog.splice(index, 1);
        renderActivity();
      });

      actions.appendChild(viewBtn);
      actions.appendChild(delBtn);

      row.appendChild(main);
      row.appendChild(actions);
      list.appendChild(row);
    });
}

// ---------- ADMIN ----------

function renderAdminPanels() {
  $("#adminBooks").textContent = JSON.stringify(books, null, 2);
  $("#adminRooms").textContent = JSON.stringify(bookings, null, 2);
  $("#adminLaptops").textContent = JSON.stringify(laptops, null, 2);
}

// ---------- AUTH ----------

function openAuthModal() {
  $("#authModal").classList.add("visible");
}

function closeAuthModal() {
  $("#authModal").classList.remove("visible");
}

function setupAuth() {
  const loginBtn = $("#loginBtn");
  const logoutBtn = $("#logoutBtn");
  const greeting = $("#userGreeting");
  const modal = $("#authModal");

  loginBtn.addEventListener("click", openAuthModal);

  $("#closeAuthModal").addEventListener("click", closeAuthModal);
  modal.addEventListener("click", (e) => {
    if (e.target === modal) closeAuthModal();
  });

  logoutBtn.addEventListener("click", () => {
    currentUser = null;
    greeting.classList.add("hidden");
    loginBtn.classList.remove("hidden");
    logoutBtn.classList.add("hidden");
  });

  $("#signupToggle").addEventListener("change", (e) => {
    $("#authTitle").textContent = e.target.checked ? "Sign up" : "Login";
  });

  $("#authSubmitBtn").addEventListener("click", () => {
    const email = $("#authEmail").value.trim();
    if (!email) {
      alert("Please enter your email.");
      return;
    }

    currentUser = { email };
    greeting.textContent = `Hi, ${email.split("@")[0]}`;
    greeting.classList.remove("hidden");
    loginBtn.classList.add("hidden");
    logoutBtn.classList.remove("hidden");
    closeAuthModal();
  });
}

// ---------- DETAILS MODAL ----------

function openDetailsModal(title, html) {
  $("#detailsTitle").textContent = title;
  $("#detailsBody").innerHTML = html;
  $("#detailsModal").classList.add("visible");
}

function closeDetailsModal() {
  $("#detailsModal").classList.remove("visible");
}

function setupDetailsModal() {
  const modal = $("#detailsModal");
  $("#closeDetailsModal").addEventListener("click", closeDetailsModal);
  modal.addEventListener("click", (e) => {
    if (e.target === modal) closeDetailsModal();
  });
}

// ---------- GLOBAL SEARCH ----------

function setupGlobalSearch() {
  $("#globalSearchBtn").addEventListener("click", () => {
    const q = $("#globalSearchInput").value.trim();
    if (!q) return;
    showSection("browse");
    renderBooks(q);
  });
}

// ---------- THEME (simple toggle: light / slight tinted) ----------

function setupThemeToggle() {
  const btn = $("#themeToggle");
  let light = true;
  btn.addEventListener("click", () => {
    light = !light;
    if (light) {
      document.body.style.background = "#f5f7fb";
      btn.textContent = "☀️";
    } else {
      document.body.style.background = "#e0e7ff";
      btn.textContent = "🌙";
    }
  });
}

// ---------- INIT ----------

window.addEventListener("DOMContentLoaded", () => {
  setupNavigation();
  setupAuth();
  setupDetailsModal();
  setupGlobalSearch();
  setupThemeToggle();

  ensureRoomDate();
  renderBooks();
  renderLaptops();
  renderRooms();
  renderWishlist();
  renderActivity();
  renderAdminPanels();

  // Book search
  $("#bookSearchBtn").addEventListener("click", () =>
    renderBooks($("#bookSearchInput").value)
  );

  // Laptop search
  $("#laptopSearchBtn").addEventListener("click", () =>
    renderLaptops($("#laptopSearchInput").value)
  );

  // Rooms
  $("#checkAvailabilityBtn").addEventListener("click", renderRooms);
  $("#roomDate").addEventListener("change", renderRooms);

  // Clear activity
  $("#clearActivityBtn").addEventListener("click", () => {
    if (!requireLogin()) return;
    if (!activityLog.length) return;
    if (confirm("Clear all activity?")) {
      activityLog.splice(0, activityLog.length);
      renderActivity();
    }
  });

  // Clear wishlist
  $("#clearWishlistBtn").addEventListener("click", () => {
    if (!requireLogin()) return;
    if (!wishlist.length) return;
    if (confirm("Clear your wishlist?")) {
      wishlist.splice(0, wishlist.length);
      renderWishlist();
    }
  });

  // Force first-time login modal (optional – comment out if annoying)
  openAuthModal();
});