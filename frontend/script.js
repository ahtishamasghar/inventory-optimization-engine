const API_URL = "http://127.0.0.1:8080/api/products";
const MAX_INTEGER = 2147483647;
const numberFormat = new Intl.NumberFormat(undefined, { maximumFractionDigits: 20 });
const priceFormat = new Intl.NumberFormat(undefined, { minimumFractionDigits: 2, maximumFractionDigits: 20 });
const modal = document.getElementById("product-modal");
const form = document.getElementById("product-form");
const message = document.getElementById("form-message");
const saveButton = document.getElementById("save-btn");
const deleteModal = document.getElementById("delete-modal");
const searchInput = document.getElementById("search-id");
let editingProductId = null;
let currentView = { mode: "all" };
let requestVersion = 0;
let connectionVersion = 0;
let savingProduct = false;
let deletingProduct = false;
let pendingDeletion = null;
let toastTimer;

function icon(name) {
    const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
    svg.classList.add("icon");
    svg.setAttribute("aria-hidden", "true");
    const use = document.createElementNS("http://www.w3.org/2000/svg", "use");
    use.setAttribute("href", `#icon-${name}`);
    svg.appendChild(use);
    return svg;
}

function setConnection(state) {
    document.getElementById("connection-state").dataset.state = state;
    document.getElementById("connection-label").textContent = {
        loading: "Connecting", online: "API connected", offline: "API unavailable"
    }[state];
}

async function apiRequest(url, options, allowMissing = false) {
    let response;
    const connectionRequest = ++connectionVersion;
    try {
        response = await fetch(url, options);
    } catch {
        if (connectionRequest === connectionVersion) setConnection("offline");
        throw new Error("Cannot reach the inventory API. Check that the backend is running at 127.0.0.1:8080.");
    }
    if (connectionRequest === connectionVersion) setConnection("online");
    if (allowMissing && response.status === 404) return null;
    if (!response.ok) {
        const detail = await response.text();
        throw new Error(detail || `The request failed (${response.status}). Please try again.`);
    }
    return response;
}

async function getProducts(url) {
    const response = await apiRequest(url);
    const data = await response.json();
    if (!Array.isArray(data.products)) throw new Error("The API returned an invalid product list.");
    return data.products;
}

async function getProduct(url) {
    const response = await apiRequest(url, undefined, true);
    return response ? response.json() : null;
}

function setBusy(button, busy, label) {
    button.disabled = busy;
    button.classList.toggle("is-busy", busy);
    button.setAttribute("aria-busy", String(busy));
    if (label) button.textContent = label;
}

function setLoading(loading) {
    const table = document.getElementById("inventory-table");
    table.classList.toggle("is-loading", loading);
    table.setAttribute("aria-busy", String(loading));
    document.getElementById("product-list").inert = loading;
    document.getElementById("inventory-stats").setAttribute("aria-busy", String(loading));
    document.getElementById("result-count").textContent = loading ? "Loading inventory…" : document.getElementById("result-count").textContent;
}

function showLoadError(text) {
    document.getElementById("load-error-text").textContent = text;
    document.getElementById("load-error").hidden = !text;
}

function updateTotals(products) {
    document.getElementById("total-products").textContent = numberFormat.format(products.length);
    document.getElementById("catalog-count").textContent = numberFormat.format(products.length);
    document.getElementById("total-stock").textContent = numberFormat.format(products.reduce((total, product) => total + product.quantity, 0));
    const lowStock = products.filter(product => product.quantity <= product.minStock).length;
    document.getElementById("low-stock").textContent = numberFormat.format(lowStock);
    document.getElementById("low-stock").closest(".card").dataset.stock = products.some(product => product.quantity === 0) ? "out" : lowStock ? "low" : "ok";
}

function emptyRow(title, description, symbol = "box") {
    const row = document.createElement("tr");
    const cell = document.createElement("td");
    cell.colSpan = 8;
    cell.className = "empty-cell";
    const content = document.createElement("div");
    content.className = "table-empty";
    const mark = document.createElement("span");
    mark.className = "empty-icon";
    mark.appendChild(icon(symbol));
    const heading = document.createElement("p");
    heading.className = "empty-title";
    heading.textContent = title;
    const copy = document.createElement("p");
    copy.className = "empty-copy";
    copy.textContent = description;
    content.append(mark, heading, copy);
    cell.appendChild(content);
    row.appendChild(cell);
    return row;
}

function renderProducts(products) {
    const fragment = document.createDocumentFragment();
    products.forEach(product => {
        const row = document.createElement("tr");
        const values = [product.id, product.name, product.category, priceFormat.format(product.price), numberFormat.format(product.quantity), numberFormat.format(product.minStock)];
        values.forEach((value, index) => {
            const cell = document.createElement("td");
            if (index >= 3) cell.className = "numeric";
            if (index === 1) cell.className = "product-name";
            if (index === 2) {
                const category = document.createElement("span");
                category.className = "category-label";
                category.textContent = value;
                category.title = value;
                cell.appendChild(category);
            } else {
                cell.textContent = value;
            }
            row.appendChild(cell);
        });
        const stock = product.quantity === 0 ? "out" : product.quantity <= product.minStock ? "low" : "ok";
        row.dataset.stock = stock;
        const statusCell = document.createElement("td");
        const badge = document.createElement("span");
        badge.className = `stock-badge stock-${stock}`;
        badge.textContent = { out: "Out of Stock", low: "Low Stock", ok: "In Stock" }[stock];
        statusCell.appendChild(badge);
        const actions = document.createElement("td");
        const group = document.createElement("div");
        group.className = "row-actions";
        [["Edit", "edit", () => openEditForm(product)], ["Delete", "trash", () => deleteProduct(product.id, product.name)]].forEach(([label, symbol, handler]) => {
            const button = document.createElement("button");
            button.type = "button";
            button.className = `button ${label === "Edit" ? "edit-btn" : "delete-btn"}`;
            button.setAttribute("aria-label", `${label} ${product.name} (ID ${product.id})`);
            button.append(icon(symbol), document.createTextNode(label));
            button.addEventListener("click", handler);
            group.appendChild(button);
        });
        actions.appendChild(group);
        row.append(statusCell, actions);
        fragment.appendChild(row);
    });
    if (!products.length) {
        const filtered = ["search-id", "search-name"].includes(currentView.mode);
        fragment.appendChild(emptyRow(filtered ? "No products found" : "Your inventory is empty", filtered ? "Try a different search or choose Show All." : "Add your first product to get started.", filtered ? "search" : "box"));
    }
    document.getElementById("product-list").replaceChildren(fragment);
    updateTableViewport();
    document.getElementById("result-count").textContent = `${numberFormat.format(products.length)} product${products.length === 1 ? "" : "s"} shown`;
}

function updateViewLabel(view) {
    const labels = { all: "All products", price: "Price · Low to high", id: "Product ID · BST inorder traversal", lowest: "Lowest stock · Min-Heap", "search-id": `Product ID · ${view.query}`, "search-name": `Name matches · “${view.query}”` };
    document.getElementById("search-message").textContent = labels[view.mode];
    document.getElementById("sort-price-btn").setAttribute("aria-pressed", String(view.mode === "price"));
    document.getElementById("sort-id-btn").setAttribute("aria-pressed", String(view.mode === "id"));
    document.getElementById("lowest-stock-btn").setAttribute("aria-pressed", String(view.mode === "lowest"));
    const headings = document.querySelectorAll("thead th");
    headings[0].removeAttribute("aria-sort");
    headings[3].removeAttribute("aria-sort");
    if (view.mode === "price") headings[3].setAttribute("aria-sort", "ascending");
    if (view.mode === "id") headings[0].setAttribute("aria-sort", "ascending");
}

async function refreshView(view = currentView) {
    const version = ++requestVersion;
    currentView = { ...view };
    setLoading(true);
    showLoadError("");
    updateViewLabel(view);
    try {
        let products;
        let totals;
        if (["all", "price", "id"].includes(view.mode)) {
            const suffix = { all: "", price: "/sorted-by-price", id: "/sorted-by-id" }[view.mode];
            products = await getProducts(`${API_URL}${suffix}`);
            totals = products;
        } else {
            const result = (async () => {
                if (view.mode === "search-name") return getProducts(`${API_URL}/search?name=${encodeURIComponent(view.query)}`);
                if (view.mode === "lowest") {
                    const lowest = await getProduct(`${API_URL}/lowest-stock`);
                    if (!lowest) return [];
                    // Keep the existing Hash Table lookup after Min-Heap retrieval.
                    const product = await getProduct(`${API_URL}/${lowest.id}`);
                    return product ? [product] : [];
                }
                const product = await getProduct(`${API_URL}/${view.query}`);
                return product ? [product] : [];
            })();
            [totals, products] = await Promise.all([getProducts(API_URL), result]);
        }
        if (version !== requestVersion) return false;
        updateTotals(totals);
        renderProducts(products);
        if (view.mode === "lowest" && products.length) {
            document.getElementById("search-mode").value = "id";
            configureSearchInput();
            searchInput.value = products[0].id;
        }
        return true;
    } catch (error) {
        if (version !== requestVersion) return false;
        showLoadError(error.message);
        document.getElementById("result-count").textContent = "Could not refresh inventory";
        document.getElementById("search-message").textContent = "View unavailable · Retry to refresh";
        // Do not present stale rows as the result of a failed search or sort.
        document.getElementById("product-list").replaceChildren(emptyRow("Inventory could not be loaded", "Check the connection and select Retry.", "alert"));
        return false;
    } finally {
        if (version === requestVersion) setLoading(false);
    }
}

function loadProducts(sortByPrice = false, sortById = false) {
    return refreshView({ mode: sortById ? "id" : sortByPrice ? "price" : "all" });
}

function configureSearchInput() {
    const byName = document.getElementById("search-mode").value === "name";
    searchInput.placeholder = byName ? "Enter product name" : "Enter Product ID";
    searchInput.setAttribute("aria-label", byName ? "Search by product name" : "Search by Product ID");
    searchInput.inputMode = byName ? "text" : "numeric";
    searchInput.setCustomValidity("");
}

function searchProduct() {
    const input = searchInput.value.trim();
    const id = Number(input);
    if (!input || !Number.isInteger(id) || id < 0 || id > MAX_INTEGER) {
        searchInput.setCustomValidity(!input ? "Please enter a Product ID." : "Enter a whole Product ID between 0 and 2147483647.");
        searchInput.reportValidity();
        return;
    }
    return refreshView({ mode: "search-id", query: id });
}

function searchProductsByName() {
    const name = searchInput.value.trim();
    if (!name) {
        searchInput.setCustomValidity("Please enter a product name.");
        searchInput.reportValidity();
        return;
    }
    return refreshView({ mode: "search-name", query: name });
}

function showToast(text) {
    clearTimeout(toastTimer);
    document.getElementById("toast-message").textContent = text;
    document.getElementById("toast").hidden = false;
    toastTimer = setTimeout(() => { document.getElementById("toast").hidden = true; }, 6000);
}

function openDialog(dialog) {
    dialog.classList.remove("is-closing");
    if (!dialog.open) dialog.showModal();
    document.body.classList.add("modal-open");
}

async function closeDialog(dialog) {
    if (!dialog.open || dialog.classList.contains("is-closing")) return;
    dialog.classList.add("is-closing");
    if (!window.matchMedia("(prefers-reduced-motion: reduce)").matches) {
        await new Promise(resolve => setTimeout(resolve, 160));
    }
    dialog.close();
    dialog.classList.remove("is-closing");
    document.body.classList.remove("modal-open");
}

function setFormError(text) {
    message.textContent = text;
    message.hidden = !text;
}

function openEditForm(product) {
    if (savingProduct) return;
    editingProductId = product.id;
    form.reset();
    document.getElementById("modal-title").textContent = "Edit Product";
    saveButton.textContent = "Update Product";
    for (const [field, value] of Object.entries({ id: product.id, name: product.name, category: product.category, price: product.price, quantity: product.quantity, "min-stock": product.minStock })) {
        document.getElementById(`product-${field}`).value = value;
    }
    document.getElementById("product-id").disabled = true;
    document.getElementById("id-hint").textContent = "Product ID is locked while editing.";
    setFormError("");
    openDialog(modal);
    document.getElementById("product-name").focus();
}

function deleteProduct(id, name) {
    if (deletingProduct) return;
    pendingDeletion = { id, name };
    document.getElementById("delete-description").textContent = `${name} · Product ID ${id}`;
    document.getElementById("delete-message").hidden = true;
    openDialog(deleteModal);
}

form.addEventListener("submit", async event => {
    event.preventDefault();
    if (savingProduct || !form.reportValidity()) return;
    const product = {
        id: Number(document.getElementById("product-id").value),
        name: document.getElementById("product-name").value.trim(),
        category: document.getElementById("product-category").value.trim(),
        price: Number(document.getElementById("product-price").value),
        quantity: Number(document.getElementById("product-quantity").value),
        minStock: Number(document.getElementById("product-min-stock").value)
    };
    if (![product.id, product.quantity, product.minStock].every(value => Number.isInteger(value) && value >= 0 && value <= MAX_INTEGER) || !Number.isFinite(product.price) || product.price < 0 || !product.name) {
        setFormError("Enter a product name, a non-negative price, and whole stock values between 0 and 2147483647.");
        return;
    }
    const isEditing = editingProductId !== null;
    const idleLabel = isEditing ? "Update Product" : "Save Product";
    savingProduct = true;
    setFormError("");
    setBusy(saveButton, true, isEditing ? "Updating…" : "Saving…");
    document.getElementById("cancel-btn").disabled = true;
    document.getElementById("close-modal-btn").disabled = true;
    try {
        await apiRequest(isEditing ? `${API_URL}/${editingProductId}` : API_URL, {
            method: isEditing ? "PUT" : "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(product)
        });
        await closeDialog(modal);
        showToast(isEditing ? "Product updated successfully." : "Product added successfully.");
        await refreshView();
    } catch (error) {
        setFormError(error.message);
    } finally {
        savingProduct = false;
        setBusy(saveButton, false, idleLabel);
        document.getElementById("cancel-btn").disabled = false;
        document.getElementById("close-modal-btn").disabled = false;
    }
});

document.getElementById("confirm-delete-btn").addEventListener("click", async () => {
    if (!pendingDeletion || deletingProduct) return;
    deletingProduct = true;
    const button = document.getElementById("confirm-delete-btn");
    setBusy(button, true, "Deleting…");
    document.getElementById("cancel-delete-btn").disabled = true;
    try {
        await apiRequest(`${API_URL}/${pendingDeletion.id}`, { method: "DELETE" });
        await closeDialog(deleteModal);
        showToast("Product deleted successfully.");
        pendingDeletion = null;
        await refreshView();
    } catch (error) {
        const label = document.getElementById("delete-message");
        label.textContent = error.message;
        label.hidden = false;
    } finally {
        deletingProduct = false;
        setBusy(button, false, "Delete Product");
        document.getElementById("cancel-delete-btn").disabled = false;
    }
});

document.getElementById("add-product-btn").addEventListener("click", () => {
    if (savingProduct) return;
    editingProductId = null;
    form.reset();
    document.getElementById("product-id").disabled = false;
    document.getElementById("modal-title").textContent = "Add New Product";
    document.getElementById("id-hint").textContent = "Unique identifier for this product.";
    saveButton.textContent = "Save Product";
    setFormError("");
    openDialog(modal);
    document.getElementById("product-id").focus();
});

for (const id of ["cancel-btn", "close-modal-btn"]) {
    document.getElementById(id).addEventListener("click", () => { if (!savingProduct) closeDialog(modal); });
}
document.getElementById("cancel-delete-btn").addEventListener("click", () => { if (!deletingProduct) closeDialog(deleteModal); });
for (const dialog of [modal, deleteModal]) {
    dialog.addEventListener("cancel", event => {
        event.preventDefault();
        if (dialog === modal ? !savingProduct : !deletingProduct) closeDialog(dialog);
    });
}
document.getElementById("search-form").addEventListener("submit", event => {
    event.preventDefault();
    if (document.getElementById("search-mode").value === "name") searchProductsByName();
    else searchProduct();
});
searchInput.addEventListener("input", () => searchInput.setCustomValidity(""));
document.getElementById("search-mode").addEventListener("change", () => {
    searchInput.value = "";
    configureSearchInput();
    loadProducts();
});
document.getElementById("clear-search-btn").addEventListener("click", () => {
    searchInput.value = "";
    searchInput.setCustomValidity("");
    loadProducts();
});
document.getElementById("sort-price-btn").addEventListener("click", () => loadProducts(true));
document.getElementById("sort-id-btn").addEventListener("click", () => loadProducts(false, true));
document.getElementById("lowest-stock-btn").addEventListener("click", () => refreshView({ mode: "lowest" }));
document.getElementById("retry-btn").addEventListener("click", () => refreshView());
document.getElementById("dismiss-toast-btn").addEventListener("click", () => { document.getElementById("toast").hidden = true; });

// Keep empty states within the visible table viewport on narrow screens.
const tableViewport = document.getElementById("inventory-table");
function updateTableViewport() {
    tableViewport.style.setProperty("--table-view-width", `${tableViewport.clientWidth}px`);
    document.getElementById("table-scroll-hint").hidden = tableViewport.scrollWidth <= tableViewport.clientWidth;
}
new ResizeObserver(updateTableViewport).observe(tableViewport);

// First-load placeholders contain no fabricated product data or metrics.
for (let rowIndex = 0; rowIndex < 4; rowIndex++) {
    const row = document.createElement("tr");
    for (let cellIndex = 0; cellIndex < 8; cellIndex++) {
        const cell = document.createElement("td");
        const skeleton = document.createElement("span");
        skeleton.className = "skeleton";
        skeleton.setAttribute("aria-hidden", "true");
        cell.appendChild(skeleton);
        row.appendChild(cell);
    }
    document.getElementById("product-list").appendChild(row);
}
loadProducts();
