def pytest_addoption(parser):
    parser.addoption(
        "--tkn",
        action="store",
        default=None,
        help="Path to the tkn executable"
    )

