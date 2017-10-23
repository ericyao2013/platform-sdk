#include <airmap/rest/rulesets.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <fmt/printf.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string airmap::rest::RuleSets::default_route_for_version(Client::Version version) {
  static constexpr const char* pattern{"/rules/%s"};

  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v1");
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage");
  }

  throw std::logic_error{"should not reach here"};
}

airmap::rest::RuleSets::RuleSets(const std::shared_ptr<net::http::Requester>& requester) : requester_{requester} {
}

void airmap::rest::RuleSets::search(const Search::Parameters& parameters, const Search::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/", std::move(query), std::move(headers), [cb](const net::http::Requester::Result& result) {
    if (result) {
      cb(jsend::to_outcome<std::vector<RuleSet>>(json::parse(result.value().body)));
    } else {
      cb(Search::Result{result.error()});
    }
  });
}

void airmap::rest::RuleSets::for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;

  requester_->get(fmt::sprintf("/%s", parameters.id), std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<RuleSet>(json::parse(result.value().body)));
                    } else {
                      cb(ForId::Result{result.error()});
                    }
                  });
}

void airmap::rest::RuleSets::fetch_rules(const FetchRules::Parameters& parameters, const FetchRules::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/rule", std::move(query), std::move(headers), [cb](const net::http::Requester::Result& result) {
    if (result) {
      cb(jsend::to_outcome<std::vector<RuleSet>>(json::parse(result.value().body)));
    } else {
      cb(FetchRules::Result{result.error()});
    }
  });
}

void airmap::rest::RuleSets::evaluate_rulesets(const Evaluation::Parameters& parameters,
                                               const Evaluation::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/evaluation", std::move(query), std::move(headers),
                  [cb](const net::http::Requester::Result& result) {
                    if (result) {
                      cb(jsend::to_outcome<std::vector<RuleSet>>(json::parse(result.value().body)));
                    } else {
                      cb(Evaluation::Result{result.error()});
                    }
                  });
}
