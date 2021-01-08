#include <chrono>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <azure/identity/client_secret_credential.hpp>
#include <azure/storage/blobs.hpp>

std::string get_config(char const *name)
{
    if (const auto var = std::getenv(name); var && *var)
    {
        return std::string{var};
    }
    else
    {
        auto ss = std::ostringstream{};
        ss << "environment variable missing or empty: " << name;
        throw std::runtime_error{ss.str()};
    }
}

using namespace std::chrono_literals;

int main(void)
{
    auto tenant_id = get_config("ARM_TENANT_ID");
    auto client_id = get_config("ARM_CLIENT_ID");
    auto client_secret = get_config("ARM_CLIENT_SECRET");
    auto storage_account_name = get_config("ARM_STORAGE_ACCOUNT_NAME");
    auto container_name = get_config("ARM_CONTAINER_NAME");

    auto credential = std::make_shared<Azure::Identity::ClientSecretCredential>(tenant_id, client_id, client_secret);

    auto service_url = std::ostringstream{};
    service_url << "https://" << storage_account_name << ".blob.core.windows.net";

    auto client = Azure::Storage::Blobs::BlobServiceClient{service_url.str(), credential};

    auto start = Azure::Core::DateTime::Now();
    auto end = start + 1h;

    auto key = client.GetUserDelegationKey(start, end).ExtractValue();

    auto sas_builder = Azure::Storage::Sas::BlobSasBuilder{};

    sas_builder.BlobContainerName = container_name;
    sas_builder.Resource = Azure::Storage::Sas::BlobSasResource::BlobContainer;
    sas_builder.ExpiresOn = end;
    sas_builder.Protocol = Azure::Storage::Sas::SasProtocol::HttpsOnly;

    sas_builder.SetPermissions(
        Azure::Storage::Sas::BlobContainerSasPermissions::Read |
        Azure::Storage::Sas::BlobContainerSasPermissions::Add |
        Azure::Storage::Sas::BlobContainerSasPermissions::Create |
        Azure::Storage::Sas::BlobContainerSasPermissions::Write |
        Azure::Storage::Sas::BlobContainerSasPermissions::List);

    std::cout.sync_with_stdio(false);

    std::cout << sas_builder.GenerateSasToken(key, storage_account_name);
}

