#include "request_handler.hxx"
#include "include/base/cef_macros.h"
#include "include/cef_base.h"

#include <fmt/core.h>

const char* app_overlay_html = "<!DOCTYPE html><html lang=\"en\"><head><style>body {overflow: hidden;background: none;}.root {position: absolute;z-index: 1;top: 0px;left: 0px;bottom: 0px;right: 0px;pointer-events: none;}.grid {position: absolute;z-index: 2;top: 0px;left: 0px;bottom: 0px;right: 0px;display: grid;grid-template-columns: 4px auto 4px;grid-template-rows: 4px auto 4px;}.frame {grid-row: 2;grid-column: 2;min-width: 0;min-height: 0;}.button-root {user-select: none;-webkit-user-select: none;position: absolute;z-index: 3;top: 0px;right: 0px;pointer-events: all;}.button {background-position: 0px 0px;width: 12px;height: 12px;cursor: pointer;float: right;}.button:hover {background-position: 0px 50%;}.button-close {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAIAAADZF8uwAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAWdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjA76PVpAAABG0lEQVQoU2MISCtyiUp1DggDkq5hcUCGo6cfhAQioKC6mQ1DUE61Dw9HMjcrBGVxsUEQhAuUsgtNZAhIzgVyanjZmgU5MBFQyiEmF2QdRNGXY2m7s0w6BdmBCMgAciGK7COSGfxi06I5WYCKdqZo/7uYAZQGIgijiY8dqMg2MIYhtKwVogii7s+xUCACMoAqgCJARVZeISDrgIpKuEGKtsXI/d5pD0RABkQbUArkJqBJQWwsQO+sDxT9tVpjc4gYEEEYQJ1ARVDfQRS9nyy8wZcHKAFEQAaQCxQESoGsA4aTLwsL0G4gAoUQOzScgIx0dlagFMjhQN+Zc3ACOXAE1A1nA6VAJgEDHkhZ+0cBSQgCagVyIQjItXD2AgAqr3dA/w8AcgAAAABJRU5ErkJggg==);}.button-close:hover {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAIAAADZF8uwAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAWdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjA76PVpAAABDElEQVQoU2MISCtyiUp1DggDkq5hcUCGo6cfhAQioKC6mQ1DUE51rZTwSlkhrKhZkMMuNJEhIDkXyLmsLnlFWwqIbuhIQxCEC5RyiMkFWQdkAfl/78x63hYNUQdk/H20GKLIPiKZwS82bbEiSBFQ4v/L1UASzgAKAqVsA2MYQstagawzhhIQdUADgAjIuGwIshEoZeUVArIOogiIHjcHAS0FIiADIgKUArkJaNJsZYHDRmIP6r3+Xu8FknAGUBFQCuo7iKIfR8sgEkAEZAC5QEGgFMg6YDhNVeHfZyAKFEJDQEGgFMjhQN9lSQsAOXAE1A1nA6VAJgEDHkhZ+0cBSQgCagVyIQjItXD2AgAcYqbcnkRY2wAAAABJRU5ErkJggg==);}.button-minimise {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAIAAADZF8uwAAAABGdBTUEAALGPC/xhBQAAAAlwSFlzAAAOwgAADsIBFShKgAAAABh0RVh0U29mdHdhcmUAcGFpbnQubmV0IDQuMC4zjOaXUAAAAQRJREFUKFNjCEgrcolKdQ4IA5KuYXFAhqOnH4QEIqCgupkNQ1BOtQ8PRzI3KwRlcbFBEIQLlLILTWQISM4Fcmp42ZoFOTARUMohJhdkHUQRBHUKssMRRJF9RDKDX2xaNCcLVkVNfOxARbaBMQyhZa1wRWgqgCJARVZeISDrgIpKuNEVQbQBpUBuApoUxMZSIc5+KEn0bIEkEB1NFzuRKX4yU7xFhgOoCOo7oKISYfZd4ULHMsSOZoidLpK4UCoJZDcqcgKlQNYBw8mXhQVoNxCBQogdGk5ARjo7K1AK5HCg78w5OIEcOALqhrOBUiCTgAEPpKz9o4AkBAG1ArkQBORaOHsBALf8ZiQ6QTlEAAAAAElFTkSuQmCC);}.button-minimise:hover {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAIAAADZF8uwAAAABGdBTUEAALGPC/xhBQAAAAlwSFlzAAAOwgAADsIBFShKgAAAABh0RVh0U29mdHdhcmUAcGFpbnQubmV0IDQuMC4zjOaXUAAAAP9JREFUKFNjCEgrcolKdQ4IA5KuYXFAhqOnH4QEIqCgupkNQ1BOda2U8EpZIayoWZDDLjSRISA5F8i5rC55RVsKiG7oSEMQhAuUcojJBVkHZAH5lw2l0RBEkX1EMoNfbNpiRZyKgFK2gTEMoWWtQNYZQwlMdRBFVl4hIOsgioAIWQVEBCgFchPQpNnKAiecFJ/3x7yemw5EL6Ymvpqe/Gpq8kVXVaAU1HdA1jEbmSfNwS+nJ72Ymfx6UfabpblA9hlvDaAUyDpgOE1V4d9nIHrYSAwNAQWBUiCHA32XJS0A5MARUDecDZQCmQQMeCBl7R8FJCEIqBXIhSAg18LZCwBF3JdMaou5EAAAAABJRU5ErkJggg==);}.button-restore {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAIAAADZF8uwAAAABGdBTUEAALGPC/xhBQAAAAlwSFlzAAAOwQAADsEBuJFr7QAAABh0RVh0U29mdHdhcmUAcGFpbnQubmV0IDQuMC45bDN+TgAAAPNJREFUKFNlkM1Kw1AQRu8iv7UKLhT6BH0DW6rVSuqioU3TYEKJUdDQVMSAuOhGV5a+mAt3fSfPZUIICh+TO/nO3Jk7Kizex+nKCxPiTfLA4XoSSET87J5dqOj1Y9p28gNT9NKyRJJiXcaPKsxLks9Da3vs/BfWKCt1O4HQ/q3z83wqEX0/nWBdLXMV3Bd3rlFDzWvggIaLTMWbHRBFEE19HdkCDfxb3Q6Iv82xhKMSS8/ETZFlUPQHkgGAqtcBUQSHJ7GGsHQ79jQzDHojvSG72hOHtW1i6cF5Xc9xSWpRXZ+x9E0sns/5PCWKKCUVkfY9/xdN1HCUE+rmYQAAAABJRU5ErkJggg==);}.button-restore:hover {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAIAAADZF8uwAAAABGdBTUEAALGPC/xhBQAAAAlwSFlzAAAOwQAADsEBuJFr7QAAABh0RVh0U29mdHdhcmUAcGFpbnQubmV0IDQuMC45bDN+TgAAAPJJREFUKFNjCEgrcolKdQ4IA5KuYXFAhqOnH4QEIqCgupkNQ1BOda2U8EpZIayoWZDDLjSRISA5F8i5rC55RVsKiG7oSEMQhAuUcojJBVkHZAH5lw2lnzQHv5yeBEFANhABpewjkhn8YtMWK0IVAeWAJBwBuUAp28AYhtCyViALqOn13HRkBFdk5RUCsg7IQjMDqAhiI1AK5CagSbOVBTAVAckzhhJAKajvgCyIM4FKIQjIBqqAKAJZBwynqSr8+wxEDxuJoSGgIFAK5HCg77KkBYAcOALqhrOBUiCTgAEPpKz9o4AkBAG1ArkQBORaOHsBADPvoaWt2fvZAAAAAElFTkSuQmCC);}.button-settings {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAIAAADZF8uwAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAWdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjA76PVpAAABNklEQVQoU2MISCtyiUp1DggDkq5hcUCGo6cfhAQioKC6mQ1DUE61Dw9HMjcrBGVxsUEQhAuUsgtNZAhIzgVyanjZmgU5IGhtoNKZBosuGR4gGyjlEJMLsg6iqFOQHYgmyPF82Oo3z0SsiY+9XYIbKGUfkczgF5sWzcmCrOjVGq/DBQYzjcQOFxoAFdkGxjCElrVCFEHUrXSV/bIr4NN2PyA510YKqMjKKwRkHVBRCTdI0cYg5a+7AxZZS/bLcLdKcQNFgFIgNwFNCmJjyeFh25qkBVSxxlsBYi9QGxABFUF9B1S0JVXnZIP5+ig1iBwEAQMCKAWyDhhOviws8/0V18ZrZAMl2KHhBGSks7MCpUAOB/rOnIMTyIEjoG44GygFMgkY8EDK2j8KSEIQUCuQC0FAroWzFwBMKnFPV14iVwAAAABJRU5ErkJggg==);}.button-settings:hover {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAIAAADZF8uwAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAWdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjA76PVpAAABM0lEQVQoU2MISCtyiUp1DggDkq5hcUCGo6cfhAQioKC6mQ1DUE51rZTwSlkhrKhZkMMuNJEhIDkXyLmsLnlFWwqIbuhIP8nzfje/4Kq5MpALlHKIyQVZB2RBpIHomqXyz0vT7wWZAdlXjRWAUvYRyQx+sWmLFRGKblmqfD814VVfElDdy0nJQCnbwBiG0LJWIOuMoQRE3cNEp1/XZv26MgNI3ouxBUpZeYWArIMoAqInBT6/b8x+EG5900LlvJ0SUAQoBXIT0KTZygKHTSSeNEcAVTzKcIfYC9EGlIL6Dsh62B79en7eo5pgiBwEHTYSA0qBrAOG01QV/puFng8bQw8biwEl4GifgShQCuRwoO+ypAWAHDgC6oazgVIgk4ABD6Ss/aOAJAQBtQK5EATkWjh7AQAo+aA8ybNcgAAAAABJRU5ErkJggg==);}.button-drag {-webkit-app-region: drag;float: right;background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAAICAIAAABsw6g0AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAWdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjA76PVpAAAAtklEQVQoU42QwQrCMBBE8wFeVPBQEaUFKQSKSAliBQ/toejBg0f//z98YWQJQbEwbCazu5Psuu72zHB5vCz+QtvfM8Wt6mbt22XpMyx2e5HyEKgx3TArKhE5ODXA5ptPokjarNrKdBUhAqVcdeyUA3B/7omgDtdtcxKRkqaUTRujEX+Rq6adviO+o6mjEe+Yse1FsDqNIM6wuloZwCQaoWJp6vQdARrh0SgMI8fXRfzdkTh6GMY3Hudnb38/oMEAAAAASUVORK5CYII=);width: 24px;height: 8px;}.border {user-select: none;-webkit-user-select: none;}.border-l {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAQAAAAWCAIAAABojaq5AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAWdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjA76PVpAAAAfUlEQVQoU2PQtnIWVtKWVNVVNrIGcYQU1ARllREcIAIyGIACQGVABJVB50hrm4CUwWVAHA0zewQHRQbCQdEDtAeoHsQBCiOUQeyFygBZUBmgAFADQhlCxsDODcExcfaGmAZyKNwLKHqAShgUTJzFNPSkNPSgMkCVILdZOQMA+jotjpzqThUAAAAASUVORK5CYII=);}.border-r {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAQAAAAWCAIAAABojaq5AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAWdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjA76PVpAAAAe0lEQVQoU2NQNrKWVNUVVtLWtnIGcQRllYUU1EAcIAayoBygDFANEAEZCA5UmbS2CYKDIgPhaJjZY8gg9ABlgZYgZIAsoCRUBsgB2oaQQVEG1IbNaAjHwM4N6lCgaSbO3ggZkBeAAhAOVI+Uhp6Yhp6CCZgD1ACy1MoZAPclLY6jSQNKAAAAAElFTkSuQmCC);}.border-t {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABYAAAAECAIAAAAFyFj8AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAWdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjA76PVpAAAAa0lEQVQoU2Ow9gpGQzb+0XASFzJx9oazGcQ09KS1TYSVtNGQkIIahKFkYAZUAxeHIx4pZQiDAaIUaIqgrDJcGwQBBSGagQw4G6gTwoWLMygbWQORhpk9hAFHQBFtK2cgQhOU17OAMKCCRtYAukswvfdQ9E4AAAAASUVORK5CYII=);}.border-b {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABYAAAAECAIAAAAFyFj8AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAWdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjA76PVpAAAAZElEQVQoU2Mwcfa29grGimz8o7Gy0UQYhBTUhJW0gUha2wSC4FwIA46AIpKqukBSSkMPWRZqBFwIKA1hABGPlDKEgaYGwoDLMigbWQORtpUznAQiDTN7eT0LCAMigiyFImtkDQDsBzDhSlW6lwAAAABJRU5ErkJggg==);}.border-tl {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAQAAAAECAIAAAAmkwkpAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAWdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjA76PVpAAAAMElEQVQYV2Nwi0y39goGIqfAaAYTZ29hJW0gElJQY9C2coawBGWVQRxpbROQpJI2ABMaCRKyKdoLAAAAAElFTkSuQmCC);}.border-tr {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAQAAAAECAIAAAAmkwkpAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAWdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjA76PVpAAAAMElEQVQYV2NwCoy29goGIrfIdAYhBTVhJW0gMnH2ZhCUVYbwta2cGSDC0tom2lbOAAiLCRL7josKAAAAAElFTkSuQmCC);}.border-bl {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAQAAAAECAIAAAAmkwkpAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAWdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjA76PVpAAAAK0lEQVQYV2NQNrKW1jYBIiCDQdvKWVhJG4JAHIgwiGPm4gHka5jZKxtZAwDTtwiG7egucAAAAABJRU5ErkJggg==);}.border-br {background: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAQAAAAECAIAAAAmkwkpAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAWdEVYdFNvZnR3YXJlAHBhaW50Lm5ldCA0LjA76PVpAAAAKUlEQVQYV2NQNrKW1jYBIiCDQVhJG4K0rZxBHIgkiANkaZjZA1lmLh4AzQYIhuYW+/oAAAAASUVORK5CYII=);}</style></head><body><div style=\"position: absolute; left: 0px; top: 0px; padding: 0px; width: 100%; height: 100%;\"><iframe src=\"https://adamcake.com/\" title=\"Bolt App\" style=\"position: relative; width: 100%; height: 100%; box-sizing:border-box;\" id=\"app-frame\"></iframe></div><div class=\"root\"><div class=\"grid\"><div class=\"border border-tl\"></div><div class=\"border border-t\"></div><div class=\"border border-tr\"></div><div class=\"border border-l\"></div><div></div><div class=\"border border-r\"></div><div class=\"border border-bl\"></div><div class=\"border border-b\"></div><div class=\"border border-br\"></div></div><div class=\"button-root\"><div class=\"button button-close\" onclick=\"window.close()\"></div><div class=\"button button-minimise\"></div><div class=\"button button-settings\"></div><div class=\"button-drag\"></div></div></div></body></html>";

struct ResourceHandler: public CefResourceHandler {
	ResourceHandler(const char* data): data(data), data_len(strlen(data)), cursor(0) { }

	bool Open(CefRefPtr<CefRequest>, bool& handle_request, CefRefPtr<CefCallback>) override {
		handle_request = true;
		return true;
	}

	void GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl) override {
		response->SetStatus(200);
		response->SetMimeType("text/html");
		response_length = this->data_len;
	}

	bool Read(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefResourceReadCallback>) override {
		if (this->cursor == this->data_len) {
			// \"To indicate response completion set |bytes_read| to 0 and return false.\"
			bytes_read = 0;
			return false;
		}
		if (this->cursor + bytes_to_read <= this->data_len) {
			// requested read is entirely in bounds
			bytes_read = bytes_to_read;
			memcpy(data_out, this->data + this->cursor, bytes_read);
			this->cursor += bytes_to_read;
		} else {
			// read only to end of string
			bytes_read = this->data_len - this->cursor;
			memcpy(data_out, this->data + this->cursor, bytes_read);
			this->cursor = this->data_len;
		}
		return true;
	}

	bool Skip(int64 bytes_to_skip, int64& bytes_skipped, CefRefPtr<CefResourceSkipCallback>) override {
		if (this->cursor + bytes_to_skip <= this->data_len) {
			// skip in bounds
			bytes_skipped = bytes_to_skip;
			this->cursor += bytes_to_skip;
		} else {
			// skip to end of string
			bytes_skipped = this->data_len - this->cursor;
			this->cursor = this->data_len;
		}
		return true;
	}

	void Cancel() override {
		this->cursor = this->data_len;
	}

	private:
		const char* data;
		size_t data_len;
		size_t cursor;
		IMPLEMENT_REFCOUNTING(ResourceHandler);
		DISALLOW_COPY_AND_ASSIGN(ResourceHandler);
};

Browser::RequestHandler::RequestHandler() {
	// Creating CefString usually allocates, so they are cached for efficiency
	this->app_overlay_url = "http://bolt/app";
}

CefRefPtr<CefResourceRequestHandler> Browser::RequestHandler::GetResourceRequestHandler(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	bool is_navigation,
	bool is_download,
	const CefString& request_initiator,
	bool& disable_default_handling
) {
	if (request->GetURL() == this->app_overlay_url) {
		disable_default_handling = true;
		return this;
	}
	return nullptr;
}

CefRefPtr<CefResourceHandler> Browser::RequestHandler::GetResourceHandler(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefRequest>) {
	return new ResourceHandler(app_overlay_html);
}